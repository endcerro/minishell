/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandorvar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 15:29:01 by hpottier          #+#    #+#             */
/*   Updated: 2020/05/06 19:07:00 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	freevarsreste(char ***vars, int size)
{
	while ((*vars)[size])
	{
		free((*vars)[size]);
		++size;
	}
	free((*vars));
	*vars = NULL;
}

void	addvar(char ***vars, char *str)
{
	int		size;
	char	**newvars;

	size = 0;
	if (*vars != NULL)
		while ((*vars)[size])
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
	if (*vars != NULL)
	{
		size = -1;
		while ((*vars)[++size])
		{
			if (!(newvars[size] = ft_strdup((*vars)[size])))
			{
				freevarsreste(vars, size);
				while (--size >= 0)
					free(newvars[size]);
				free(newvars);
				ft_putstr("minishell: ");
				ft_putendl(strerror(errno));
				return ;
			}
			free((*vars)[size]);
		}
		free(*vars);
	}
	*vars = newvars;
}

char	*checkpath(char **envi, char **params)
{
	int			x;
	int			i;
	int			prev;
	struct stat	sta;
	char		*str;

	x = 0;
	while (envi[x] && ft_strncmp(envi[x], "PATH=", 5) != 0)
		++x;
	if (envi[x] == 0)
		return (NULL); // Meme sans PATH bash test un truc par defaut
	i = 4;
	prev = 5;
	while (envi[x][++i])
		if (envi[x][i] == ':')
		{
			envi[x][i] = 0;
			str = ft_strjoinf1(ft_strjoin(&envi[x][prev], "/"), params[0]);
			envi[x][i] = ':';
			if (stat(str, &sta) != -1)
				return (str);
			free(str);
			prev = i + 1;
		}
	return (NULL);
}

int		checkslash(char *file)
{
	int x;

	x = -1;
	while (file[++x])
		if (file[x] == '/')
			return (1);
	return (0);
}

void	execsomestuff(char **envi, char **vars, char **params)
{
	struct stat	sta;
	char		*str;

	(void)vars;// Remplacer vars par leurs valeurs dans params
	str = NULL;
	if (checkslash(params[0]))
	{
		if (stat(params[0], &sta) == -1)
		{
			ft_putstr("minishell: ");
			ft_putstr(params[0]);
			ft_putstr(": No such file or directory\n");
			return ;
		}
	}
	else
		if ((str = checkpath(envi, params)) == NULL)
		{
			ft_putstr("minishell: ");
			ft_putstr(params[0]);
			ft_putstr(": command not found\n");
			return ;
		}
	g_mshell.pid = fork();
	if (g_mshell.pid == 0)
	{
		if (str)
			execve(str, params, envi);
		else
			execve(params[0], params, envi);
		ft_putstr("minishell: ");
		ft_putstr(params[0]);
		ft_putstr(": ");
		ft_putendl(strerror(errno));
		exit(0);
	}
	else if (g_mshell.pid < 0)
	{
		ft_putstr("minishell: ");
		ft_putstr(params[0]);
		ft_putstr(": ");
		ft_putendl(strerror(errno));
		return ;
	}
	// Gerer les signals
	/* wait(&g_mshell.exitcode); */
	while (waitpid(g_mshell.pid, &g_mshell.exitcode, WNOHANG) == 0)
		;
	/* write(1, "\n", 1); */
}

void	commandorvar(char ***envi, char **params, char ***vars)
{
	int x;
	int i;

	x = -1;

	// char ***envi = &(g_mshell.env);
	// char **params = g_mshell.params;
	// char ***vars = &(g_mshell.params);

	while (params[++x])
	{
		i = 0;
		while (params[x][++i])
			if (params[x][i] == '=' && params[x][i - 1] != '\\')
			{
				addvar(vars, params[x]);
				break ;
			}
		if (params[x][i] == 0)
		{
			execsomestuff(*envi, *vars, &(x[params]));
			break ;
		}
	}
}
