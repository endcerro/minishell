/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandorvar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 15:29:01 by hpottier          #+#    #+#             */
/*   Updated: 2020/07/10 21:03:15 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	freevarsreste(int size)
{
	while (g_mshell.vars[size])
	{
		free(g_mshell.vars[size]);
		++size;
	}
	free(g_mshell.vars);
	g_mshell.vars = NULL;
}

int		addvarbis(char **newvars, int size, char *str)
{
	if (!(newvars[size] = ft_strdup(str)))
	{
		free(newvars);
		return (ft_printh(2, 1, "minishell: %s\n", strerror(errno)));
	}
	if (g_mshell.vars != NULL)
	{
		size = -1;
		while (g_mshell.vars[++size])
			if (!(newvars[size] = ft_strdup(g_mshell.vars[size])))
			{
				freevarsreste(size);
				while (--size >= 0)
					free(newvars[size]);
				free(newvars);
				return (ft_printh(2, 1, "minishell: ", strerror(errno)));
			}
			else
				free(g_mshell.vars[size]);
		free(g_mshell.vars);
	}
	g_mshell.vars = newvars;
	return (0);
}

int		addvar(char *str)
{
	int		size;
	char	**newvars;
	char	*tmp;

	if (env(str) != NULL)
		return (export(str));
	size = 0;
	if (g_mshell.vars != NULL)
		while (g_mshell.vars[size])
		{
			if (ft_strncmp(g_mshell.vars[size], str,
						ft_strchr(str, '=') - str) == 0)
			{
				if ((tmp = ft_strdup(str)) == NULL)
					return (ft_printh(2, 1, "minishel: %s\n", strerror(errno)));
				free(g_mshell.vars[size]);
				g_mshell.vars[size] = tmp;
				return (0);
			}
			++size;
		}
	if (!(newvars = (char **)xmalloc(sizeof(char *) * (size + 2))))
		return (ft_printh(2, 1, "minishell: %s\n", strerror(errno)));
	newvars[size + 1] = NULL;
	return (addvarbis(newvars, size, str));
}

char	*checkpathbis(char **params, int j, int x)
{
	int			i;
	int			prev;
	struct stat	sta;
	char		*str;

	i = 4;
	prev = 5;
	while (g_mshell.env[x][++i])
		if (g_mshell.env[x][i] == ':')
		{
			g_mshell.env[x][i] = 0;
			str = ft_strjoinf1(ft_strjoin(&g_mshell.env[x][prev], "/"),
							params[j]);
			g_mshell.env[x][i] = ':';
			if (stat(str, &sta) != -1)
				return (str);
			free(str);
			prev = i + 1;
		}
	return (NULL);
}

char	*checkpath(int j, char **params)
{
	int			x;
	struct stat	sta;
	char		*str;

	x = 0;
	while (g_mshell.env[x] && ft_strncmp(g_mshell.env[x], "PATH=", 5) != 0)
		++x;
	if (g_mshell.env[x] == 0)
	{
		str = ft_strjoin("/bin/", params[j]);
		if (stat(str, &sta) != -1)
			return (str);
		return (NULL);
	}
	return (checkpathbis(params, j, x));
}

int		checkslash(char *str)
{
	int x;

	x = -1;
	while (str[++x])
		if (str[x] == '/')
			return (1);
	return (0);
}

void	execsomestuffbis(int x, char **params, char *str)
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
		return ;
	}
	free(str);
	while (waitpid(g_mshell.pid, &g_mshell.exitcode, WNOHANG) == 0)
		;
}

void	execsomestuff(int x, char **params)
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
			return ;
		}
	}
	else
	{
		if ((str = checkpath(x, params)) == NULL)
		{
			ft_printh(2, 1, "minishell: %s: command not found\n", params[x]);
			return ;
		}
	}
	g_mshell.pid = fork();
	execsomestuffbis(x, params, str);
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
	if ((out = xmalloc(sizeof(char*) * (i + 1))) == NULL)
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
				addvar(params[x]);
				break ;
			}
		if (params[x][i] == 0)
		{
			execsomestuff(x, params);
			break ;
		}
	}
	free(params);
	return (0);
}
