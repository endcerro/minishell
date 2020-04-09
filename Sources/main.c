/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/16 20:40:33 by edal--ce          #+#    #+#             */
/*   Updated: 2020/04/03 07:43:14 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prompt(char **line)
{
	ft_putstr("\033[31mminishell \033[33m@>\033[0m");
	get_next_line(0, line);
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
	while (ptr[i])
	{
		free(ptr[i]);
		++i;
	}
	free(ptr);
}

void	echo(char **params)	// Ne gere ni les escape ni les quotes
{
	int i;
	int ret;

	i = 0;
	ret = 1;
	if (params[1] && ft_strcmp(params[1], "-n") == 0)
	{
		++i;
		ret = 0;
	}
	while (params[++i])
	{
		ft_putstr(params[i]);
		if (params[i + 1])
			write(1, " ", 1);
	}
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

void	env(char **envi, char **params)
{
	int x;

	x = 0;
	if (params[1])
		ft_putstr("minishell: env: too many arguments\n");
	else
	{
		while (envi[x])
		{
			if (checkexport(envi[x]) == 1)
				ft_putsendl(envi[x]);
			++x;
		}
	}
}

char	*rethomedir(char **envi)
{
	while (*envi && ft_strncmp("HOME=", *envi, 5) != 0)
		++envi;
	if (*envi)
		return (*envi + 5);
	return (NULL);
}

/* char	*parse_dot_dot(char *pwd) */
/* { */
/* 	t_path 	**path; */
/* 	t_path 	*curr; */
/* 	t_path 	*tmp; */

/* 	tmp = NULL; */
/* 	path = init_path(pwd + 4); */
/* 	curr = *path; */
/* 	while(curr && ft_strncmp(curr->str,"..", 2) == 0) */
/* 	{ */
/* 		*path = curr->next; */
/* 		free(curr); */
/* 		curr = *path; */
/* 	} */
/* 	while(curr) */
/* 	{ */
/* 		if(curr->next && ft_strncmp(curr->next->str,"..", 2) == 0) */
/* 		{ */
/* 			if(tmp) */
/* 			{ */
/* 				tmp->next = curr->next->next; */
/* 				free(curr->next); */
/* 				free(curr); */
/* 				curr = tmp->next; */
/* 			} */
/* 			else */
/* 			{ */
/* 				*path = curr->next->next; */
/* 				free(curr->next); */
/* 				free(curr); */
/* 				curr = *path; */
/* 			} */
/* 		} */
/* 		else */
/* 		{ */
/* 			tmp = curr; */
/* 			curr = curr->next; */
/* 		} */
/* 	} */
/* 	return (join_pwd(path, pwd)); */
/* } */

void	cd(char **envi, char **params)
{
	int		oldpwd;
	int		pwd;
	int		x;
	char	*home;
	char	*str;

	if (params[1] && params[2] != 0)
	{
		ft_putstr("minishell: cd: wrond number of arguments\n");
		return ;
	}
	if (params[1])
	{
		if (chdir(params[1]) == -1)
		{
			ft_putstr(strerror(errno));
			write(1, "\n", 1);
			return ;
		}
	}
	else
	{
		if (chdir((home = rethomedir(envi))) == -1)
		{
			ft_putstr("minishell: cd: HOME not set\n");
			return ;
		}
	}
	oldpwd = -1;
	pwd = -1;
	x = -1;
	while (envi[++x] && (pwd == -1 || oldpwd == -1))
		if (ft_strncmp(envi[x], "PWD=", 4) == 0)
			pwd = x;
		else if (ft_strncmp(envi[x], "OLDPWD=", 7) == 0)
			oldpwd = x;
	if (envi[x] == NULL)
		return ;
	free(envi[oldpwd]);
	envi[oldpwd] = ft_strjoin("OLD", envi[pwd]);
	free(envi[pwd]);
	if ((str = getcwdwrap()) == NULL)
		ft_putstr(strerror(errno));
	else
		envi[pwd] = ft_strjoinf2("PWD=", str);
	/* if (params[1] && params[1][0] == '.' && params[1][1] == 0) */
	/* 	return ; */
	/* free(envi[pwd]); */
	/* if (params[1] == NULL) */
	/* 	envi[pwd] = ft_strjoinf2("PWD=", ft_strdup(home)); */
	/* else if (params[1][0] == '/') */
	/* 	envi[pwd] = ft_strjoinf2("PWD=", ft_strdup(params[1])); */
	/* else */
	/* 	envi[pwd] = ft_strjoinf2(&envi[oldpwd][3], ft_strjoinf2("/", ft_strdup(params[1]))); */
	/* envi[pwd] = parse_dot_dot(envi[pwd]); */
}

void	pwd(char **params)
{
	/* int x; */
	char *str;

	/* x = 0; */
	if (params[1])
	{
		ft_putstr("minishell: pwd: too many arguments\n");
		return ;
	}
	/* while (envi[x] && ft_strncmp(envi[x], "PWD=", 4) != 0) */
	/* 	++x; */
	/* if (envi[x]) */
	/* 	ft_putstr(&envi[x][4]); */
	str = getcwdwrap();
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
		env2[y] = ft_strdup(envi[y]);
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

void	export(char ***envi, char **params)
{
	int		i;
	char	**n_envi;

	i = 0;
	if (params[2] != 0)
	{
		ft_putstr("minishell: export: wrong number of arguments\n");
		return ;
	}
	else if (params[1] == 0)
	{
		exportlst(*envi);
		return ;
	}
	if(!ft_strnstr(params[1], "=", ft_strlen(params[1])))
		params[1] = ft_strjoinf1(params[1],"=");
	while ((*envi)[i])
	{
		if(!check_match((*envi)[i], params[1]))
		{
			free((*envi)[i]);
			(*envi)[i] = ft_strdup(params[1]);
			return ;
		}
		i++;
	}
	if(!(n_envi = malloc(sizeof(char *) * (i + 2))))
		return ;
	i = -1;
	while ((*envi)[++i])
		n_envi[i] = (*envi)[i];
	n_envi[i++] = ft_strdup(params[1]);
	n_envi[i] = 0;
	free(*envi);
	*envi = n_envi;
}

void	unset(char **envi, char **params)
{
	int		i;

	i = 0;
	if (params[1] == 0 || params[2] != 0)
	{
		ft_putstr("minishell: unset: wrong number of arguments\n");
		return;
	}
	while (envi[i])
	{
		if(!check_match(envi[i], params[1]))
		{
			free(envi[i]);
			while (envi[++i])
				envi[i - 1] = envi[i];
			envi[i - 1] = 0;
			return;
		}
		i++;
	}
}

void	checkinput(char ***envi, char **params)
{
	if (ft_strcmp(params[0], "exit") == 0) // Fini
	{
		ft_putstr("exit\n");
		if (params[1])
			ft_putstr("minishell: exit: too many arguments\n");
		freechar2ptr(params);
		freechar2ptr(*envi);
		exit(0);
	}
	else if (ft_strcmp(params[0], "echo") == 0) // A terminer
		echo(params);
	else if (ft_strcmp(params[0], "env") == 0) // Fini
		env(*envi, params);
	else if (ft_strcmp(params[0], "cd") == 0) // Fini
		cd(*envi, params);
	else if (ft_strcmp(params[0], "pwd") == 0) // Fini
		pwd(params);
	else if (ft_strcmp(params[0], "export") == 0) // A fignoler
		export(envi, params);
	else if (ft_strcmp(params[0], "unset") == 0) // A terminer
		unset(*envi, params);
}

char	**newenviron()
{
	char	**envi;
	int		x;
	/* char	keys[3]; */

	x = 0;
	/* ft_bzero(keys, 3); */
	while (environ[x])
	{
		/* if (ft_strncmp("PWD=", environ[x], 4) == 0) */
		/* 	keys[0] = 1; */
		/* if (ft_strncmp("SHLVL=", environ[x], 4) == 0) */
		/* 	keys[1] = 1; */
		/* if (ft_strncmp("_=", environ[x], 4) == 0) */
		/* 	keys[2] = 1; */
		++x;
	}
	++x;
	if ((envi = (char **)malloc(sizeof(char *) * x)) == NULL)
		return (NULL);
	x = -1;
	while (environ[++x])
		envi[x] = ft_strdup(environ[x]);
	envi[x] = NULL;
	return (envi);
}

int		main(int ac, char **av)
{
	char *line;
	char **params;
	char **envi;

	(void)ac;
	(void)av;
	envi = newenviron();
	while (1)
	{
		prompt(&line);
		if (line == NULL && *line == 0)
			break ;
		if (!(params = get_blocks(line)))
			return (0);
		free(line);
		if (*params)
			checkinput(&envi, params);
		freechar2ptr(params);
	}
}
