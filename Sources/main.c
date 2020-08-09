/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/16 20:40:33 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/08 18:40:58 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		prompt(char **line)
{
	g_mshell.pid = 0;
	if (g_mshell.linestate == 0 || g_mshell.dquote == 1)
		ft_putstr_fd("\033[31mminishell \033[33m@>\033[0m", 2);
	g_mshell.dquote = 0;
	return (get_next_line(0, line));
}

int		newenvironnext(char *keys, int x, int z)
{
	char	*str;

	if (keys[0] == 0)
		if ((g_mshell.env[x + z++] = ft_strjoinf2("PWD=", getcwdwrap())) == 0)
			return (freechar2ptr(g_mshell.env, -1));
	if (keys[1] == 0)
		if ((g_mshell.env[x + z++] = ft_strdup("OLDPWD")) == NULL)
			return (freechar2ptr(g_mshell.env, -1));
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

int		newenviron(void)
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

void	init_mshell(void)
{
	g_mshell.pid = 0;
	g_mshell.exitcode = 0;
	g_mshell.vars = NULL;
	g_mshell.ls = 0;
	g_mshell.oldfdout = dup(1);
	g_mshell.oldfdin = dup(0);
	g_mshell.sigswitch = 0;
	g_mshell.linestate = 0;
	g_mshell.dquote = 0;
}

int		main(void)
{
	char *line;

	init_mshell();
	signal(SIGINT, &sigint);
	signal(SIGQUIT, &sigquit);
	if (newenviron() == -1)
		return (1);
	unset("OLDPWD");
	export("OLDPWD");
	mainloop(0, NULL, &line);
	write(2, "exit\n", 5);
	if (g_mshell.ls)
		ft_lstclear(&(g_mshell.ls));
	free(line);
	freechar2ptr(g_mshell.env, 0);
	freechar2ptr(g_mshell.vars, 0);
	/* system("leaks minishell"); */
}
