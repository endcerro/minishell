/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/16 20:40:33 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/26 15:47:10 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		prompt(char **line)
{
	ft_putstr("\033[31mminishell \033[33m@>\033[0m");
	return (get_next_line(0, line));
}

char	**getfiller(int depth, char *cpt)		//MALLOC PROTECTED
{
	char *tmp;
	char **out;

	out = 0;

	write(1, "dquote> ", 8);
	get_next_line(0, &tmp);
	if (tmp == NULL)
		return (NULL);
	escape_chars(tmp);
	parse_qts(tmp, cpt);
	check_pipe(tmp, cpt);
//	parse_bs(tmp);
	if (cpt[0] % 2 || cpt[1] % 2 || cpt[2])
	{
		out = getfiller(depth + 1, cpt);
		if (out == 0)
			return ((char **)(long)freeret(tmp, 0));
	}
	else
	{
		if(!(out = malloc(sizeof(char *) * (depth + 2))))
		{
			free(tmp);
			return (NULL);
		}
		out[depth + 1] = 0;
	}
	if(depth == 0)
	{
		tmp = ft_strjoinf2("\n", tmp);
		if(tmp == 0)
		{
			free (out);
			return (0);
		}
	}
	out[depth] = tmp;
	return (out);
}

int		checkexport(char *var)
{
	int x;

	x = 0;
	while (var[x] && var[x] != '=')
		++x;
	if (var[x] == 0)
		return (0);
	return (1);
}

char 	*env(char *request)
{
	int x;

	x = 0;
	if (request == NULL && g_mshell.ls->next && g_mshell.ls->next->type == 1)
	{
		ft_putstr_fd("minishell: env: too many arguments\n", 2);
		return (NULL);
	}
	while (g_mshell.env[x])
	{
		if(request != NULL)
		{
			if (!ft_strncmp(g_mshell.env[x], request, wordlen(request)))
				return (g_mshell.env[x] + wordlen(request));
		}
		else if (checkexport(g_mshell.env[x]) == 1)
			ft_putsendl(g_mshell.env[x]);
		++x;
	}
	return (NULL);
}

char 	*vars(char *request)
{
	int x;

	if (request != NULL && g_mshell.vars != NULL)
	{
		x = 0;
		while (g_mshell.vars[x])
		{
			if (!ft_strncmp(g_mshell.vars[x], request, ft_strlen(request)))
				return (g_mshell.vars[x] + ft_strlen(request) + 1);
			++x;
		}
	}
	return (NULL);
}

int		newenviron(void) // Il faut gerer la variable _= qui n'apparait que dans env
{
	int		x;
	int		z;
	char	keys[3];

	x = 0;
	z = 3;
	ft_bzero(keys, 3);
	while (environ[x])
	{
		if (ft_strncmp("PWD=", environ[x], 4) == 0)
		{
			keys[0] = 1;
			--z;
		}
		if (ft_strncmp("SHLVL=", environ[x], 4) == 0)
		{
			keys[1] = 1;
			--z;
		}
		if (ft_strncmp("OLDPWD=", environ[x], 4) == 0)
		{
			keys[2] = 1;
			--z;
		}
		++x;
	}
	++x;
	if ((g_mshell.env = (char **)malloc(sizeof(char *) * (x + z))) == NULL)
		return (-1);
	x = -1;
	while (environ[++x])
		if ((g_mshell.env[x] = ft_strdup(environ[x])) == NULL)
		{
			freechar2ptr(g_mshell.env);
			return (-1);
		}
	g_mshell.env[x + z] = NULL;
	z = 0;
	if (keys[0] == 0)
	{
		if ((g_mshell.env[x + z] = ft_strjoinf2("PWD=", getcwdwrap())) == NULL)
		{
			freechar2ptr(g_mshell.env);
			return (-1);
		}
		++z;
	}
	if (keys[1] == 0)
	{
		if ((g_mshell.env[x + z] = ft_strdup("SHLVL=1")) == NULL)
		{
			freechar2ptr(g_mshell.env);
			return (-1);
		}
		++z;
	}
	if (keys[2] == 0)
		if ((g_mshell.env[x + z] = ft_strdup("OLDPWD")) == NULL)
		{
			freechar2ptr(g_mshell.env);
			return (-1);
		}
	return (0);
}

void	sigint(int sig)
{
	if (g_mshell.pid != 0)
	{
		kill(g_mshell.pid, sig);
		g_mshell.pid = 0;
		/* write(1, "\n", 1); */
	}
	g_mshell.exitcode = 130;
	/* signal(sig, SIG_IGN); */
	/* write(1, "\n", 1); */
}

void	sigquit(int sig)
{
	if (g_mshell.pid != 0)
	{
		kill(g_mshell.pid, sig);
		g_mshell.pid = 0;
		/* write(1, "\n", 1); */
	}
	g_mshell.exitcode = 131;
	/* signal(sig, SIG_IGN); */
	/* write(1, "\n", 1); */
}

void 	init_mshell(void)
{
	g_mshell.pid = 0;
	g_mshell.exitcode = 0;
	g_mshell.vars = NULL;
	g_mshell.ls = 0;
	g_mshell.pipes[2] = -1;
	g_mshell.pipes[5] = -1;
	g_mshell.rdirin = 0;
	g_mshell.rdirout = 0;
	g_mshell.oldfdout = dup(1);
	g_mshell.oldfdin = dup(0);
}

int		main(void)
{
	char *line;

	init_mshell();
	signal(SIGINT, &sigint);
	signal(SIGQUIT, &sigquit);
	if (newenviron() == -1)
		return (1);
	while (prompt(&line) > 0)
	{
		// if (line == NULL && *line == 0)
		if (line == NULL)
			break ;
		line = get_lst(line); // PROTECTED UNTILL HERE
		if (line == NULL)
			continue ;
		checkinput_ls(line);
		ft_lstclear(&g_mshell.ls);
		free(line);
		line = 0;
	}
	write(1, "exit\n", 5);
	if(g_mshell.ls)
		ft_lstclear(&(g_mshell.ls));
	free(line);
	freechar2ptr(g_mshell.env);
	freechar2ptr(g_mshell.vars);
}
