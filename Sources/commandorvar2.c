/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandorvar2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/11 14:39:46 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/28 20:54:56 by hpottier         ###   ########.fr       */
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
	if (!(newvars = (char **)malloc(sizeof(char *) * (size + 2))))
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
			if (*(params[j]) == '\0')
				return (NULL);
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
