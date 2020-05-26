/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/16 20:40:33 by edal--ce          #+#    #+#             */
/*   Updated: 2020/05/26 15:37:20 by edal--ce         ###   ########.fr       */
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





char	**getfiller(int depth, int *cpt)
{
	char *tmp;
	char **out;

	write(1, "dquote> ", 8);
	get_next_line(0, &tmp);
	// printf("tmp b4 :%s\n",tmp);
	parse_qts(tmp, cpt);
	out = 0;
	// printf("tmp af %d :%s\n", *cpt,tmp);
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

int 	wordlen(char *str)
{
	int i;

	i = -1;
	while (str[++i])
	{
		if (!ft_isalnum(str[i]))
			return i;
	}
	return i;
}

char 	*env(char *request)
{
	int x;

	x = 0;
	// if (g_mshell.params[1] && request == NULL)
	// 	ft_putstr("minishell: env: too many arguments\n");
	// else
//	{
		while (g_mshell.env[x])
		{
			if(request != NULL)
			{
				if (!ft_strncmp(g_mshell.env[x], request, wordlen(request)))
					return (g_mshell.env[x] + wordlen(request));
			}
			else if (checkexport(g_mshell.env[x]) == 1)
			{
				// printf("here\n");
				ft_putsendl(g_mshell.env[x]);
			}
			++x;
		}
//	}
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
	t_list	*curr;

	curr = g_mshell.ls->next;

	if (curr && curr->next != 0)
	{
		ft_putstr("minishell: cd: wrond number of arguments\n");
		return ;
	}
	if (curr)
	{
		if (chdir(curr->content) == -1)
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
	t_list	*curr;

	curr = g_mshell.ls->next;

	if (curr)
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
	freechar2ptr(env2);
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
	char	**n_envi;
	t_list	*curr;

	if (g_mshell.ls)
		curr = g_mshell.ls->next;
	i = 0;

	if(check_valid_export(params) == 0) //FUNC TO DO
	{
		ft_putstr("export not valid identifier\n");
		return;
	}
	if (curr == 0)
	{
		exportlst(g_mshell.env); // PAS PROTEGE
		return ;
	}
	if(!ft_strnstr(curr->content, "=", ft_strlen(curr->content)))
	{
		curr->content = ft_strjoinf1(curr->content,"="); // PAS PROTEGE
	}
	while (g_mshell.env[i])
	{
		if(!check_match(g_mshell.env[i], curr->content))
		{
			free(g_mshell.env[i]);
			g_mshell.env[i] = ft_strdup(curr->content); // PAS PROTEGE
			return ;
		}
		i++;
	}
	if(!(n_envi = malloc(sizeof(char *) * (i + 2))))
		return ;
	i = -1;
	while (g_mshell.env[++i])
		n_envi[i] = g_mshell.env[i];
	n_envi[i++] = ft_strdup(curr->content); // PAS PROTEGE
	n_envi[i] = 0;
	free(g_mshell.env);
	g_mshell.env = n_envi;
}

void	unset()
{
	int		i;
	t_list	*curr;

	curr = g_mshell.ls->next;

	i = 0;
	if (curr == 0 || curr->next != 0)
	{
		ft_putstr("minishell: unset: wrong number of arguments\n");
		return;
	}
	while (g_mshell.env[i])
	{
		if(!check_match(g_mshell.env[i], curr->content))
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

int		newenviron() // Il faut gerer la variable _= qui n'apparait que dans env
{
	int		x;
	int		z;
	char	keys[3];
	char	*(str[3]);

	x = 0;
	z = 3;
	ft_bzero(keys, 3);
	str[2] = NULL;
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
			freechar2ptr(env);
			return (-1);
		}
	g_mshell.env[x + z] = NULL;
	z = 0;
	if (keys[0] == 0)
	{
		if ((g_mshell.env[x + z] = ft_strjoinf2("PWD=", getcwdwrap())) == NULL)
		{
			freechar2ptr(env);
			return (-1);
		}
		++z;
	}
	if (keys[1] == 0)
	{
		if ((g_mshell.env[x + z] = ft_strdup("SHLVL=1")) == NULL)
		{
			freechar2ptr(env);
			return (-1);
		}
		++z;
	}
	if (keys[2] == 0)
		if ((g_mshell.env[x + z] = ft_strdup("OLDPWD=")) == NULL)
		{
			freechar2ptr(env);
			return (-1);
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
	g_mshell.vars = NULL;
	signal(SIGINT, &sigkill);
	signal(SIGQUIT, &sigkill);
	if (newenviron() == -1)
		return (1);
	while (prompt(&line) > 0)
	{
		if (line == NULL && *line == 0)
			break ;
		// if (get_blocks(line) == -1) // PAS PROTEGE
		// 	break ;
		get_lst(line);

		checkinput_ls();

		free(line);
		// freechar2ptr(g_mshell.params);
	}
	ft_putstr("exit\n");
	free(line);
	freechar2ptr(g_mshell.env);
	freechar2ptr(g_mshell.vars);
}
