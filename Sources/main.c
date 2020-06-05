/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/16 20:40:33 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/05 19:10:38 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		prompt(char **line)
{
	ft_putstr("\033[31mminishell \033[33m@>\033[0m");
	return (get_next_line(0, line));
}

char	*getcwdwrap(void)
{
	char			*str;
	unsigned int	i;

	i = 256;
	str = NULL;
	while (i < UINT_MAX)
	{
		if (!(str = (char *)malloc(i)))
			return (NULL);
		if (getcwd(str, i) != NULL)
			break ;
		free(str);
		i += 100;
	}
	return (str);
}

char	**getfiller(int depth, int *cpt)
{
	char *tmp;
	char **out;

	write(1, "dquote> ", 8);
	get_next_line(0, &tmp);
	parse_qts(tmp, cpt);
	check_pipe(tmp, cpt);
	out = 0;
	parse_bs(tmp);
	if (cpt[0] % 2 || cpt[1] % 2)
		out = getfiller(depth + 1, cpt); // PAS PROTEGE
	else
	{
		out = malloc(sizeof(char *) * (depth + 2));
		out[depth + 1] = 0;
	}
	if(depth == 0)
		tmp = ft_strjoinf2("\n", tmp); // PAS PROTEGE
	out[depth] = tmp;
	return (out);
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

char	*rethomedir(void)
{
	int	x;

	x = 0;
	while (g_mshell.env[x] && ft_strncmp("HOME=", g_mshell.env[x], 5) != 0)
		++x;
	if (g_mshell.env[x])
		return (g_mshell.env[x] + 5);
	return (NULL);
}

int		cd(void)
{
	int		oldpwd;
	int		pwd;
	int		x;
	char	*home;
	char	*str;
	t_list	*curr;

	curr = g_mshell.ls->next;

	if (curr && curr->next != 0)
	{
		ft_putstr("minishell: cd: wrond number of arguments\n");
		return (1);
	}
	if (curr)
	{
		if (chdir(curr->content) == -1)
		{
			ft_putstr(strerror(errno));
			write(1, "\n", 1);
			return (1);
		}
	}
	else
	{
		if (chdir((home = rethomedir())) == -1)
		{
			ft_putstr("minishell: cd: $HOME not set\n");
			return (1);
		}
	}
	oldpwd = -1;
	pwd = -1;
	x = -1;
	while (g_mshell.env[++x] && (pwd == -1 || oldpwd == -1))
		if (ft_strncmp(g_mshell.env[x], "PWD=", 4) == 0)
			pwd = x;
		else if (ft_strncmp(g_mshell.env[x], "OLDPWD=", 7) == 0)
			oldpwd = x;
	if (g_mshell.env[x] == NULL)
		return (0);
	free(g_mshell.env[oldpwd]);
	g_mshell.env[oldpwd] = ft_strjoin("OLD", g_mshell.env[pwd]); // PAS PROTEGE
	free(g_mshell.env[pwd]);
	if ((str = getcwdwrap()) == NULL)
		ft_putstr(strerror(errno));
	else
		g_mshell.env[pwd] = ft_strjoinf2("PWD=", str); // PAS PROTEGE
	return (0);
}

int		pwd(void)
{
	char *str;

	if (g_mshell.ls->next && g_mshell.ls->next->type == 1)
	{
		ft_putstr("minishell: pwd: too many arguments\n");
		return (1);
	}
	if ((str = getcwdwrap()) == NULL)
	{
		ft_putstr("minishell: pwd: ");
		ft_putstr(strerror(errno));
		return (1);
	}
	ft_putstr(str);
	write(1, "\n", 1);
	free(str);
	return (0);
}

int		unset(void)
{
	int		i;
	t_list	*curr;

	curr = g_mshell.ls->next;

	i = 0;
	if (curr == 0 || curr->next != 0)
	{
		ft_putstr("minishell: unset: wrong number of arguments\n");
		return (1);
	}
	while (g_mshell.env[i])
	{
		if(!check_match(g_mshell.env[i], curr->content))
		{
			free(g_mshell.env[i]);
			while (g_mshell.env[++i])
				g_mshell.env[i - 1] = g_mshell.env[i];
			g_mshell.env[i - 1] = 0;
			return (0);
		}
		i++;
	}
	return (0);
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

int		main(void)
{
	char *line;

	g_mshell.pid = 0;
	g_mshell.exitcode = 0;
	g_mshell.vars = NULL;
	g_mshell.ls = 0;

	g_mshell.pipe1[2] = -1;
	g_mshell.pipe2[2] = -1;
	g_mshell.pipes[2] = -1;
	g_mshell.pipes[5] = -1;
	g_mshell.rdirin = 0;
	g_mshell.rdirout = 0;
	// g_mshell.pipnb = 0;

	g_mshell.oldfdout = dup(1);
	g_mshell.oldfdin = dup(0);

	signal(SIGINT, &sigint);
	signal(SIGQUIT, &sigquit);
	if (newenviron() == -1)
		return (1);
	while (prompt(&line) > 0)
	{
		if (line == NULL && *line == 0)
			break ;

		line = get_lst(line);
		checkinput_ls();
		ft_lstclear(&g_mshell.ls);
		free(line);
		line = 0;
	}
	ft_putstr("exit\n");
	if(g_mshell.ls)
		ft_lstclear(&(g_mshell.ls));
	free(line);
	freechar2ptr(g_mshell.env);
	freechar2ptr(g_mshell.vars);
}
