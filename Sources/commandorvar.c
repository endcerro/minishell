/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandorvar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 15:29:01 by hpottier          #+#    #+#             */
/*   Updated: 2020/08/08 18:17:57 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		checkslash(char *str)
{
	int x;

	x = -1;
	while (str[++x])
		if (str[x] == '/')
			return (1);
	return (0);
}

int		execsomestuffbis(int x, char **params, char *str)
{
	if (g_mshell.pid == 0)
	{
		if (str)
			execve(str, &(params[x]), g_mshell.env);
		else
			execve(params[x], &(params[x]), g_mshell.env);
		ft_printh(2, 1, "minishell: %s: %s\n", params[x], strerror(errno));
		free(str);
		freechar2ptr(g_mshell.env, 0);
		freechar2ptr(params, 0);
		freechar2ptr(g_mshell.vars, 0);
		exit(0);
	}
	else if (g_mshell.pid < 0)
	{
		ft_printh(2, 1, "minishell: %s: %s\n", params[x], strerror(errno));
		free(str);
		return (1);
	}
	free(str);
	waitpid(g_mshell.pid, &g_mshell.exitcode, 0);
	g_mshell.exitcode /= 256;
	if (g_mshell.sigswitch != 0)
		return (g_mshell.sigswitch);
	return (g_mshell.exitcode);
}

int		execsomestuff(int x, char **params, int *npipe, char *str)
{
	struct stat	sta;

	if (checkslash(params[x]))
	{
		if (stat(params[x], &sta) == -1)
			return (ft_printh(2, 127,
				"minishell: %s: No such file or directory\n", params[x]));
	}
	else
	{
		if ((str = checkpath(x, params)) == NULL)
			return (ft_printh(2, 127, "minishell: %s: command not found\n",
				params[x]));
	}
	if (*npipe == 0 && --(*npipe) && (g_mshell.pid = fork()))
		return (execsomestuffbis(x, params, str));
	execve(str ? str : params[x], &(params[x]), g_mshell.env);
	ft_printh(2, 1, "minishell: %s: %s\n", params[x], strerror(errno));
	freechar2ptr(g_mshell.env, 0);
	freechar2ptr(params, 0);
	freechar2ptr(g_mshell.vars, 0);
	exit(freeret(str, 0));
	return (0);
}

char	**ls_params(void)
{
	char	**out;
	int		i;
	t_list	*curr;

	curr = g_mshell.ls;
	i = 0;
	while (curr && curr->type == 1 && ++i)
		curr = curr->next;
	if ((out = malloc(sizeof(char*) * (i + 1))) == NULL)
		return (NULL);
	curr = g_mshell.ls;
	i = 0;
	while (curr && curr->type == 1)
	{
		out[i++] = curr->content;
		curr = curr->next;
	}
	out[i] = 0;
	return (out);
}

int		commandorvar(int *npipe, int i)
{
	int		x;
	char	**params;

	if ((params = ls_params()) == NULL)
		return (ft_printh(2, 1, "minishell: %s\n", strerror(errno)));
	x = -1;
	while (params[++x])
	{
		i = 0;
		while (params[x][++i])
			if (params[x][i] == '=' && params[x][i - 1] != '\\')
			{
				g_mshell.exitcode = addvar(params[x]);
				break ;
			}
		if (params[x][i] == 0)
		{
			g_mshell.exitcode = execsomestuff(x, params, npipe, 0);
			if (*npipe <= 0)
				g_mshell.sigswitch = 0;
			break ;
		}
	}
	free(params);
	return (0);
}
