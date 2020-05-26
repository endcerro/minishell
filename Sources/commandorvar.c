/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandorvar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 15:29:01 by hpottier          #+#    #+#             */
/*   Updated: 2020/05/24 20:43:23 by edal--ce         ###   ########.fr       */
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

void	addvar(char *str)
{
	int		size;
	char	**newvars;
	char	*tmp[3];

	size = 0;
	tmp[2] = NULL;
	tmp[0] = ft_strchr(str, '=');
	*tmp[0] = 0;
	if (env(str) != NULL)
	{
		*tmp[0] = '=';
		tmp[1] = str;
		export(tmp);
		return ;
	}
	*tmp[0] = '=';
	if (g_mshell.vars != NULL)
		while (g_mshell.vars[size])
			++size;
	if (!(newvars = (char **)malloc(sizeof (char *) * (size + 2))))
	{
		ft_putstr("minishell: ");
		ft_putendl(strerror(errno));
		return ;
	}
	newvars[size + 1] = NULL;
	if (!(newvars[size] = ft_strdup(str)))
	{
		free(newvars);
		ft_putstr("minishell: ");
		ft_putendl(strerror(errno));
		return ;
	}
	if (g_mshell.vars != NULL)
	{
		size = -1;
		while (g_mshell.vars[++size])
		{
			if (!(newvars[size] = ft_strdup(g_mshell.vars[size])))
			{
				freevarsreste(size);
				while (--size >= 0)
					free(newvars[size]);
				free(newvars);
				ft_putstr("minishell: ");
				ft_putendl(strerror(errno));
				return ;
			}
			free(g_mshell.vars[size]);
		}
		free(g_mshell.vars);
	}
	g_mshell.vars = newvars;
}

char	*checkpath(int j, char **params)
{
	int			x;
	int			i;
	int			prev;
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
	i = 4;
	prev = 5;
	while (g_mshell.env[x][++i])
		if (g_mshell.env[x][i] == ':')
		{
			g_mshell.env[x][i] = 0;
			str = ft_strjoinf1(ft_strjoin(&g_mshell.env[x][prev], "/"), params[j]);
			g_mshell.env[x][i] = ':';
			if (stat(str, &sta) != -1)
				return (str);
			free(str);
			prev = i + 1;
		}
	return (NULL);
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

void	execsomestuff(int x, char **params)
{
	struct stat	sta;
	char		*str;

	str = NULL;
	if (checkslash(params[x]))
	{
		if (stat(params[x], &sta) == -1)
		{
			ft_putstr("minishell: ");
			ft_putstr(params[x]);
			ft_putstr(": No such file or directory\n");
			return ;
		}
	}
	else
		if ((str = checkpath(x, params)) == NULL)
		{
			ft_putstr("minishell: ");
			ft_putstr(params[x]);
			ft_putstr(": command not found\n");
			return ;
		}
	g_mshell.pid = fork();
	if (g_mshell.pid == 0)
	{
		if (str)
			execve(str, &(params[x]), g_mshell.env);
		else
			execve(params[x], &(params[x]), g_mshell.env);
		ft_putstr("minishell: ");
		ft_putstr(params[x]);
		ft_putstr(": ");
		ft_putendl(strerror(errno));
		free(str);
		freechar2ptr(g_mshell.env);
		freechar2ptr(params);
		freechar2ptr(g_mshell.vars);
		exit(0);
	}
	else if (g_mshell.pid < 0)
	{
		ft_putstr("minishell: ");
		ft_putstr(params[x]);
		ft_putstr(": ");
		ft_putendl(strerror(errno));
		free(str);
		return ;
	}
	free(str);
	// Gerer les signals
	/* wait(&g_mshell.exitcode); */
	while (waitpid(g_mshell.pid, &g_mshell.exitcode, WNOHANG) == 0)
		;
	/* write(1, "\n", 1); */
}

char	**ls_params()
{
	char	**out;
	int		i;
	t_list	*curr;

	curr = g_mshell.ls;
	i = 0;
	while(curr && curr->type == 1 && ++i)
		curr = curr->next;
	if ((out = malloc(sizeof(char*) * (i + 1))) == NULL)
		return (NULL);
	curr = g_mshell.ls;
	i = 0;
	while(curr && curr->type == 1)
	{
		out[i++] = curr->content;
		curr = curr->next;
	}
	out[i] = 0;
	return out;
}

void	commandorvar(void)
{
	int		x;
	int		i;
	char	**params;

	if ((params = ls_params()) == NULL)
	{
		ft_putstr("minishell: ");
		ft_putendl(strerror(errno));
		return ;
	}
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
}
