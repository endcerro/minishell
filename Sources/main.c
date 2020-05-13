/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/16 20:40:33 by edal--ce          #+#    #+#             */
/*   Updated: 2020/05/06 19:14:48 by edal--ce         ###   ########.fr       */
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

void	freechar2ptr(char **ptr)
{
	int i;

	i = 0;
	if (ptr == 0)
		return ;
	while (ptr[i])
	{
		free(ptr[i]);
		++i;
	}
	free(ptr);
}

int parse_esc(char *str)
{
	int i;
	int j;
	int r;
	int tra;

	r = 0;
	tra = 0;
	j = -1;
	if (*(str + 1) == '\\')
	{
		while (*(str + r) == '\\')
			r++;
		if (*(str + r) == '"' || *(str + r) == '\'')
			r++;
		tra = (r % 2 == 0) ? r - r / 2 : r - (r + 1) / 2;
		r = -tra;
	}
	else if ((*(str + 1) == '\'' || *(str + 1) == '\"') && (tra = 1))
		*str = (*(str + 1) == '\'') ? '\'' : '\"';
	while (++j < tra)
	{
		i = -1;
		while (str[++i])
			str[i] = str[i + 1];
	}
	return (r);
}

int	parse_bs(char *str)
{
	int i;

	i = -1;
	while (str[++i])
		if (str[i] == '\\')
			i -= parse_esc(str + i);

	return 0;
}
void 	parse_qts(char *str, int *cpt)
{
	int 	i;
	int 	j;
	int 	v;

	i = 0;
	j = -1;
	while (str[++j])
		if ((str[j] == '\'' || str[j] == '\"'))
		{
			v = 0;
			if(j > 0 && str[j - 1] == '\\' && (v = 1))
			{
				while (j - v >= 0 && str[j - v] == '\\')
					v++;
				v--;
			}
			if ((v % 2 == 0) && !(cpt[str[j] == '\"'] % 2))
			{
				cpt[!(str[j] == '\"')]++;
				i = -1;
				while(str[j + ++i])
					str[j + i] = str[j + i + 1];
				j--;
			}
		}
}

char **getfiller(int depth, int *cpt)
{
	char *tmp;
	char **out;

	write(1, "dquote> ", 8);
	get_next_line(0, &tmp);
	parse_qts(tmp, cpt);
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

char **check_finished()
{
	int		i;
	int		cpt[2];
	char	**fill;

	i = -1;
	fill = 0;
	ft_bzero(cpt, 2);
	while (g_mshell.params[++i])
	{
		parse_qts(g_mshell.params[i], cpt);
		parse_env(&g_mshell.params[i]);
		parse_bs(g_mshell.params[i]);
	}
	if (cpt[0] % 2 || cpt[1] % 2)
	 	fill = getfiller(0, cpt); // PAS PROTEGE
	return (fill);
}

void	echo()	//Devrait etre pas mal, à vérifier
{
	int i;
	int ret;
	char **fill;

	i = 0;
	ret = 0;
	fill = 0;
	if (g_mshell.params[1] && ft_strcmp(g_mshell.params[1], "-n") == 0)
		++i;
	else
		ret = 1;
	fill = check_finished();
	while (g_mshell.params[++i])
	{
		ft_putstr(g_mshell.params[i]);
		if (g_mshell.params[i + 1])
			write(1," ",1);
	}
	i = -1;
	while (fill && fill[++i])
	{
		parse_env(&(fill[i]));
		ft_putstr(fill[i]);
		if (fill[i + 1])
			write(1, "\n", 1);
	}
	freechar2ptr(fill);
	if (ret)
		write(1, "\n", 1);
}

int		checkexport(char *var)
{
	int x;

	x = 0;
	while (var[x] && var[x] != '=')
		++x;
	if (var[x] == 0 || (var[x] == '=' && var[x + 1] == 0))
		return (0);
	return (1);
}

char 	*env(char *request)
{
	int x;

	x = 0;
	if (g_mshell.params[1] && request == NULL)
		ft_putstr("minishell: env: too many arguments\n");
	else
	{
		while (g_mshell.env[x])
		{
			if(request != NULL)
			{
				if (!ft_strncmp(g_mshell.env[x], request, ft_strlen(request)))
					return (g_mshell.env[x] + ft_strlen(request) + 1);
			}
			else if (checkexport(g_mshell.env[x]) == 1)
				ft_putsendl(g_mshell.env[x]);
			++x;
		}
	}
	return (NULL);
}

char	*rethomedir()
{
	int	x;

	x = 0;
	while (g_mshell.env[x] && ft_strncmp("HOME=", g_mshell.env[x], 5) != 0)
		++x;
	if (g_mshell.env[x])
		return (g_mshell.env[x] + 5);
	return (NULL);
}

void	cd()
{
	int		oldpwd;
	int		pwd;
	int		x;
	char	*home;
	char	*str;

	if (g_mshell.params[1] && g_mshell.params[2] != 0)
	{
		ft_putstr("minishell: cd: wrond number of arguments\n");
		return ;
	}
	if (g_mshell.params[1])
	{
		if (chdir(g_mshell.params[1]) == -1)
		{
			ft_putstr(strerror(errno));
			write(1, "\n", 1);
			return ;
		}
	}
	else
	{
		if (chdir((home = rethomedir())) == -1)
		{
			ft_putstr("minishell: cd: HOME not set\n");
			return ;
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
	/* printf("pwd = %d\noldpwd = %d\n", pwd, oldpwd); // AAAAAAAAAAAAAAAAAAAAAAAA */
	if (g_mshell.env[x] == NULL)
		return ;
	free(g_mshell.env[oldpwd]);
	g_mshell.env[oldpwd] = ft_strjoin("OLD", g_mshell.env[pwd]); // PAS PROTEGE
	free(g_mshell.env[pwd]);
	if ((str = getcwdwrap()) == NULL)
		ft_putstr(strerror(errno));
	else
		g_mshell.env[pwd] = ft_strjoinf2("PWD=", str); // PAS PROTEGE
}

void	pwd()
{
	char *str;

	if (g_mshell.params[1])
	{
		ft_putstr("minishell: pwd: too many arguments\n");
		return ;
	}
	if ((str = getcwdwrap()) == NULL)
	{
		ft_putstr("minishell: pwd: ");
		ft_putstr(strerror(errno));
		return ;
	}
	ft_putstr(str);
	write(1, "\n", 1);
	free(str);
}

int		parti(char **tab, int lo, int hi)
{
	char	*pivot;
	char	*tmp;
	int		i;
	int		j;

	pivot = tab[hi];
	i = (lo - 1);
	j = lo;
	while (j <= hi)
	{
		if (ft_strcmp(tab[j], pivot) < 0)
		{
			++i;
			tmp = tab[i];
			tab[i] = tab[j];
			tab[j] = tmp;
		}
		++j;
	}
	tmp = tab[i + 1];
	tab[i + 1] = tab[hi];
	tab[hi] = tmp;
	return (i + 1);
}

void	quicks(char **tab, int lo, int hi)
{
	int pi;

	if (lo < hi)
	{
		pi = parti(tab, lo, hi);
		quicks(tab, lo, pi - 1);
		quicks(tab, pi + 1, hi);
	}
}

void	exportlst(char **envi)
{
	int		x;
	int		y;
	char	**env2;

	x = 0;
	while (envi[x])
		++x;
	if ((env2 = (char **)malloc(sizeof(char *) * (x + 1))) == NULL)
	{
		ft_putstr(strerror(errno));
		return ;
	}
	y = -1;
	while (envi[++y])
		env2[y] = ft_strdup(envi[y]); // PAS PROTEGE
	env2[y] = NULL;
	quicks(env2, 0, y - 1);
	x = -1;
	while (env2[++x])
	{
		y = 0;
		ft_putstr("declare -x ");
		while (env2[x][y] != '=')
			++y;
		write(1, env2[x], y);
		if (env2[x][y + 1] != 0)
		{
			write(1, "=\"", 2);
			ft_putstr(env2[x] + y + 1);
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
	}
}

int		check_match(char *env, char *param)
{
	int ret;

	ret = ft_strncmp(env, param, ft_strlen(env)
				- ft_strlen(ft_strnstr(env, "=", ft_strlen(env))));
	return (ret);
}

void	export(char **params)
{
	int		i;
	char 	***envi = &(g_mshell.env);
	char	**n_envi;

	i = 0;
	if (params[1] != 0 && params[2] != 0)
	{
		ft_putstr("minishell: export: wrong number of arguments\n");
		return ;
	}
	else if (params[1] == 0)
	{
		exportlst(*envi); // PAS PROTEGE
		return ;
	}
	if(!ft_strnstr(params[1], "=", ft_strlen(params[1])))
		params[1] = ft_strjoinf1(params[1],"="); // PAS PROTEGE
	while ((*envi)[i])
	{
		if(!check_match((*envi)[i], params[1]))
		{
			free((*envi)[i]);
			(*envi)[i] = ft_strdup(params[1]); // PAS PROTEGE
			return ;
		}
		i++;
	}
	if(!(n_envi = malloc(sizeof(char *) * (i + 2))))
		return ;
	i = -1;
	while ((*envi)[++i])
		n_envi[i] = (*envi)[i];
	n_envi[i++] = ft_strdup(params[1]); // PAS PROTEGE
	n_envi[i] = 0;
	free(*envi);
	*envi = n_envi;
}

void	unset()
{
	int		i;

	i = 0;
	if (g_mshell.params[1] == 0 || g_mshell.params[2] != 0)
	{
		ft_putstr("minishell: unset: wrong number of arguments\n");
		return;
	}
	while (g_mshell.env[i])
	{
		if(!check_match(g_mshell.env[i], g_mshell.params[1]))
		{
			free(g_mshell.env[i]);
			while (g_mshell.env[++i])
				g_mshell.env[i - 1] = g_mshell.env[i];
			g_mshell.env[i - 1] = 0;
			return;
		}
		i++;
	}
}

int 	get_output(char **params)
{
	int i;
	int fd;

	fd = 1;
	i = -1;
	while(params[++i])
	{
		if (ft_strcmp(params[i], ">") == 0)
		{
			if (params[i + 1])
			{
				// int stdout = dup(1);

				// fd = open(params[i + 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
				// dup2(fd, 1);
				// free(params[i + 1]);
				// free(params[i]);
				// dup2(stdout, 1);
				// params[i] = 0;
				// write(fd, "File created\n", 13);
			}
		}
	}
	return (fd);
}

void	checkinput(void)
{
	if (ft_strcmp(g_mshell.params[0], "exit") == 0) // Fini
	{
		ft_putstr("exit\n");
		if (g_mshell.params[1])
			ft_putstr("minishell: exit: too many arguments\n");
		freechar2ptr(g_mshell.env);
		freechar2ptr(g_mshell.params);
		freechar2ptr(g_mshell.vars);
		exit(0);
	}
	else if (ft_strcmp(g_mshell.params[0], "echo") == 0) // A terminer
		echo();
	else if (ft_strcmp(g_mshell.params[0], "env") == 0) // Fini
		env(NULL);
	else if (ft_strcmp(g_mshell.params[0], "cd") == 0) // Fini // PAS PROTEGE
		cd();
	else if (ft_strcmp(g_mshell.params[0], "pwd") == 0) // Fini
		pwd();
	else if (ft_strcmp(g_mshell.params[0], "export") == 0) // A fignoler // PAS PROTEGE
		export(g_mshell.params);
	else if (ft_strcmp(g_mshell.params[0], "unset") == 0) // A terminer
		unset();
	else if (ft_strcmp(g_mshell.params[0], "clear") == 0)
		ft_putstr("\033c");
	else
		commandorvar();
}

int		newenviron() // Il faut gerer la variable _= qui n'apparait que dans env
{
	int		x;
	char	keys[3];
	char	*(str[3]);

	x = 0;
	ft_bzero(keys, 3);
	str[2] = NULL;
	while (environ[x])
	{
		if (ft_strncmp("PWD=", environ[x], 4) == 0)
			keys[0] = 1;
		if (ft_strncmp("SHLVL=", environ[x], 4) == 0)
			keys[1] = 1;
		if (ft_strncmp("OLDPWD=", environ[x], 4) == 0)
			keys[2] = 1;
		++x;
	}
	++x;
	if ((g_mshell.env = (char **)malloc(sizeof(char *) * x)) == NULL)
		return (-1);
	x = -1;
	while (environ[++x])
		g_mshell.env[x] = ft_strdup(environ[x]); // PAS PROTEGE
	g_mshell.env[x] = NULL;
	if (keys[0] == 0)
	{
		str[1] = ft_strjoinf2("PWD=", getcwdwrap()); // PAS PROTEGE
		export(str); // PAS PROTEGE
		free(str[1]);
	}
	if (keys[1] == 0)
	{
		str[1] = ft_strdup("SHLVL=1"); // PAS PROTEGE
		export(str); // PAS PROTEGE
		free(str[1]);
	}
	if (keys[2] == 0)
	{
		str[1] = ft_strdup("OLDPWD="); // PAS PROTEGE
		export(str); // PAS PROTEGE
		free(str[1]);
	}
	return (0);
}

void	sigkill(int sig)
{
	if (g_mshell.pid != 0)
	{
		kill(g_mshell.pid, sig);
		g_mshell.pid = 0;
		/* write(1, "\n", 1); */
	}
	/* signal(sig, SIG_IGN); */
	/* write(1, "\n", 1); */
}

int		main(void)
{
	char *line;

	g_mshell.pid = 0;
	g_mshell.exitcode = 0;
	signal(SIGINT, &sigkill);
	signal(SIGQUIT, &sigkill);
	if (newenviron() == -1) // PAS PROTEGE
		return (1);
	while (prompt(&line) > 0)
	{
		if (line == NULL && *line == 0)
			break ;
		if (get_blocks(line) == -1) // PAS PROTEGE
			return (0); // Rien n'est free en cas d'erreur
		free(line);
		if (*g_mshell.params)
			checkinput(); // PAS PROTEGE
		freechar2ptr(g_mshell.params);
	}
	ft_putstr("exit\n");
	free(line);
	freechar2ptr(g_mshell.env);
	freechar2ptr(g_mshell.vars);
}
