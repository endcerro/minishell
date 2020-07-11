/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandorvar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 15:29:01 by hpottier          #+#    #+#             */
/*   Updated: 2020/07/11 17:37:48 by hpottier         ###   ########.fr       */
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
	while (waitpid(g_mshell.pid, &g_mshell.exitcode, WNOHANG) == 0)
		;
	if (g_mshell.sigswitch != 0)
		return (g_mshell.sigswitch);
	return (g_mshell.exitcode);
}

int		execsomestuff(int x, char **params)
{
	struct stat	sta;
	char		*str;

	str = NULL;
	if (checkslash(params[x]))
	{
		if (stat(params[x], &sta) == -1)
		{
			ft_printh(2, 1, "minishell: %s: No such file or directory\n",
					params[x]);
			return (127);
		}
	}
	else
	{
		if ((str = checkpath(x, params)) == NULL)
		{
			ft_printh(2, 1, "minishell: %s: command not found\n", params[x]);
			return (127);
		}
	}
	g_mshell.pid = fork();
	return (execsomestuffbis(x, params, str));
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

int		commandorvar(void)
{
	int		x;
	int		i;
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
			g_mshell.exitcode = execsomestuff(x, params);
			g_mshell.sigswitch = 0;
			break ;
		}
	}
	free(params);
	return (0);
}
