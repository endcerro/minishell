/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/16 20:40:33 by edal--ce          #+#    #+#             */
/*   Updated: 2020/04/26 23:04:37 by edal--ce         ###   ########.fr       */
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

int parse_esc(char *str) //Faut changer cette merde
{
	int i;
	int j;
	int r;

	r = 0;
	i = 1;
	if(*(str + 1) == 'n' && (r = 1))
		*str = '\n';
	else if(*(str + 1) == 't' && (r = 1))
		*str = '\t';
	else if(*(str + 1) == 'v' && (r = 1))
		*str = '\v';
	else if(*(str + 1) == 'b' && (r = 1))
		*str = '\b';
	else if(*(str + 1) == 'r' && (r = 1))
		*str = '\r';
	else if(*(str + 1) == 'f' && (r = 1))
		*str = '\f';
	else if(*(str + 1) == 'a' && (r = 1))
		*str = '\a';
	else if(*(str + 1) == '\\' && (r = 1))
		*str = '\\';
	else if(*(str + 1) == '?' && (r = 1))
		*str = '?';
	else if(*(str + 1) == '\'' && (r = 1))
		*str = '\'';
	else if(*(str + 1) == '\"' && (r = 1))
		*str = '\"';
	j = -1;
	while(str[++j + i])
		str[j + i] = str[j + i + 1];
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

char **check_finished(char **params)
{
	int		i;
	int		cpt[2];
	char	**fill;

	i = -1;
	fill = 0;
	cpt[0] = 0;
	cpt[1] = 0;
	while (params[++i])
	{
		parse_qts(params[i], cpt);
		parse_bs(params[i]);
	}
	if (cpt[0] % 2 || cpt[1] % 2)
	 	fill = getfiller(0, cpt); // PAS PROTEGE
	return (fill);
}

void	echo(char **params)	//Devrait etre pas mal, à vérifier
{
	int i;
	int ret;
	char **fill;

	i = 0;
	ret = 1;
	fill = 0;
	fill = check_finished(params);
	if (params[1] && ft_strcmp(params[1], "-n") == 0)
	{
		++i;
		ret = 0;
	}
	while (params[++i])
	{
		ft_putstr(params[i]);
		if (params[i + 1])
			write(1," ",1);
	}
	i = -1;
	while(fill && fill[++i])
	{
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
	printf("pwd = %d\noldpwd = %d\n", pwd, oldpwd); // AAAAAAAAAAAAAAAAAAAAAAAA
	if (envi[x] == NULL)
		return ;
	free(envi[oldpwd]);
	envi[oldpwd] = ft_strjoin("OLD", envi[pwd]); // PAS PROTEGE
	free(envi[pwd]);
	if ((str = getcwdwrap()) == NULL)
		ft_putstr(strerror(errno));
	else
		envi[pwd] = ft_strjoinf2("PWD=", str); // PAS PROTEGE
}

void	pwd(char **params)
{
	char *str;

	if (params[1])
	{
		ft_putstr("minishell: pwd: too many arguments\n");
		return ;
	}
	if ((str = getcwdwrap()) != NULL)
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

void	export(char ***envi, char **params)
{
	int		i;
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

void	checkinput(char ***envi, char **params, char ***vars)
{
	if (ft_strcmp(params[0], "exit") == 0) // Fini
	{
		ft_putstr("exit\n");
		if (params[1])
			ft_putstr("minishell: exit: too many arguments\n");
		freechar2ptr(params);
		freechar2ptr(*envi);
		freechar2ptr(*vars);
		exit(0);
	}
	else if (ft_strcmp(params[0], "echo") == 0) // A terminer
		echo(params);
	else if (ft_strcmp(params[0], "env") == 0) // Fini
		env(*envi, params);
	else if (ft_strcmp(params[0], "cd") == 0) // Fini // PAS PROTEGE
		cd(*envi, params);
	else if (ft_strcmp(params[0], "pwd") == 0) // Fini
		pwd(params);
	else if (ft_strcmp(params[0], "export") == 0) // A fignoler // PAS PROTEGE
		export(envi, params);
	else if (ft_strcmp(params[0], "unset") == 0) // A terminer
		unset(*envi, params);
	else
		commandorvar(envi, params, vars);
}

char	**newenviron() // Il faut gerer la variable _= qui n'apparait que dans env
{
	char	**envi;
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
	if ((envi = (char **)malloc(sizeof(char *) * x)) == NULL)
		return (NULL);
	x = -1;
	while (environ[++x])
		envi[x] = ft_strdup(environ[x]); // PAS PROTEGE
	envi[x] = NULL;
	if (keys[0] == 0)
	{
		str[1] = ft_strjoinf2("PWD=", getcwdwrap()); // PAS PROTEGE
		export(&envi, str); // PAS PROTEGE
		free(str[1]);
	}
	if (keys[1] == 0)
	{
		str[1] = ft_strdup("SHLVL=1"); // PAS PROTEGE
		export(&envi, str); // PAS PROTEGE
		free(str[1]);
	}
	if (keys[2] == 0)
	{
		str[1] = ft_strdup("OLDPWD="); // PAS PROTEGE
		export(&envi, str); // PAS PROTEGE
		free(str[1]);
	}
	return (envi);
}

int		main(int ac, char **av)
{
	char *line;
	char **params;
	char **envi;
	char **vars;

	(void)ac;
	(void)av;
	vars = NULL;
	envi = newenviron(); // PAS PROTEGE
	while (prompt(&line) > 0)
	{
		if (line == NULL && *line == 0)
			break ;
		if (!(params = get_blocks(line)))
			return (0);
		free(line);
		if (*params)
			checkinput(&envi, params, &vars); // PAS PROTEGE
		freechar2ptr(params);
	}
	ft_putstr("exit\n");
	free(line);
	freechar2ptr(envi);
	freechar2ptr(vars);
}
