/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/16 20:40:33 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/09 20:50:43 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		prompt(char **line)									//PROTECTED
{
	ft_putstr("\033[31mminishell \033[33m@>\033[0m");
	return (get_next_line(0, line));
}

void	promptnextline(char **str)
{
	write(1, "dquote> ", 8);
	get_next_line(0, str);
}

char	**getfiller(int depth, char *cpt)		//MALLOC PROTECTED
{
	char *tmp;
	char **out;

	promptnextline(&tmp);
	if (tmp == NULL)
		return (NULL);
	escape_chars(tmp, 0, 0);
	parse_qts(tmp, cpt);
	if (cpt[0] % 2 || cpt[1] % 2 || cpt[2])
	{
		out = getfiller(depth + 1, cpt);
		if (out == 0)
			return ((char **)(long)freeret(tmp, 0));
	}
	else
	{
		if (!(out = malloc(sizeof(char *) * (depth + 2))))
			return ((char **)(long)freeret(tmp, 0));
		out[depth + 1] = 0;
	}
	if (depth == 0)
		if (!(tmp = ft_strjoinf2("\n", tmp)))
			return ((char **)(long)freeret(out, 0));
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

char	*env(char *request)
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
		if (request != NULL)
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

char	*vars(char *request)
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

int		newenvironnext(char *keys, int x, int z)
{
	char	*str;

	if (keys[0] == 0)
	{
		if ((g_mshell.env[x + z++] = ft_strjoinf2("PWD=", getcwdwrap())) == 0)
			return (freechar2ptr(g_mshell.env, -1));
	}
	if (keys[1] == 0)
	{
		if ((g_mshell.env[x + z++] = ft_strdup("OLDPWD")) == NULL)
			return (freechar2ptr(g_mshell.env, -1));
	}
	if (keys[2] == 0)
	{
		if ((g_mshell.env[x + z] = ft_strdup("SHLVL=1")) == NULL)
			return (freechar2ptr(g_mshell.env, -1));
	}
	else
	{
		str = env("SHLVL") + 1;
		if ((str = ft_itoaa(ft_atoi(str) + 1)) == NULL)
			return (freechar2ptr(g_mshell.env, -1));
		if ((str = ft_strjoinf2("SHLVL=", str)) == NULL)
			return (freechar2ptr(g_mshell.env, -1));
		export(str);
		free(str);
	}
	return (0);
}

int		newenviron(void) //Il faut gerer la variable _= qui n'apparait que dans env
{
	int			x;
	int			z;
	char		keys[3];
	extern char	**environ;

	x = -1;
	z = 3;
	ft_bzero(keys, 3);
	while (environ[++x])
	{
		if (ft_strncmp("PWD=", environ[x], 4) == 0 && z--)
			keys[0] = 1;
		if (ft_strncmp("SHLVL=", environ[x], 4) == 0 && z--)
			keys[2] = 1;
		if (ft_strncmp("OLDPWD=", environ[x], 4) == 0 && z--)
			keys[1] = 1;
	}
	if ((g_mshell.env = (char **)malloc(sizeof(char *) * (x + z + 1))) == NULL)
		return (-1);
	x = -1;
	while (environ[++x])
		if ((g_mshell.env[x] = ft_strdup(environ[x])) == NULL)
			return (freechar2ptr(g_mshell.env, -1));
	g_mshell.env[x + z] = NULL;
	return (newenvironnext(keys, x, 0));
}

void	sigint(int sig)
{
	if (g_mshell.pid != 0)
	{
		kill(g_mshell.pid, sig);
		g_mshell.pid = 0;
	}
	g_mshell.exitcode = 130;
}

void	sigquit(int sig)
{
	if (g_mshell.pid != 0)
	{
		kill(g_mshell.pid, sig);
		g_mshell.pid = 0;
	}
	g_mshell.exitcode = 131;
}

void	init_mshell(void)
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
	if (g_mshell.ls)
		ft_lstclear(&(g_mshell.ls));
	free(line);
	freechar2ptr(g_mshell.env, 0);
	freechar2ptr(g_mshell.vars, 0);
}


static int fail_after = 5;
static int num_allocs = 0;

static void *xmalloc(size_t size)
{
    if (fail_after > 0 && num_allocs++ >= fail_after)
    {
        ft_putstr("Out of memory\n");
        return 0;
    }
    return malloc(size);
}