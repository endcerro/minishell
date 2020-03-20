/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/16 20:40:33 by edal--ce          #+#    #+#             */
/*   Updated: 2020/03/21 00:44:14 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	prompt(char **line)
{
	ft_putstr("\033[31mminishell \033[33m@>\033[0m");
	get_next_line(0, line);
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
			ft_putstr(envi[x]);
			write(1, "\n", 1);
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

// int		checkdotdot(char *param)
// {
// 	(void)param;
// 	return (0);
// }

// char	*retdotdot(char *pwd, char *param)
// {
// 	(void)pwd;
// 	(void)param;
// 	return (NULL);
// }

char	*parse_dot_dot(char *pwd)
{
	t_path 	**path;
	t_path 	*curr;
	t_path 	*tmp;

	tmp = NULL;
	path = init_path(pwd + 4);
	curr = *path;

	// printf("first .. trim.....\n");
	// printf("%s\n", curr->str);
	while(curr && ft_strncmp(curr->str,"..", 2) == 0)
	{
		*path = curr->next;
		free(curr);
		curr = *path;
	}
	// printf("Done\n");
	while(curr)
	{
		if(curr->next && ft_strncmp(curr->next->str,"..", 2) == 0)
		{
			if(tmp)
			{
				tmp->next = curr->next->next;
				free(curr->next);
				free(curr);
				curr = tmp->next;
			}
			else
			{
				*path = curr->next->next;
				free(curr->next);
				free(curr);
				curr = *path;
			}
		}
		tmp = curr;
		curr = curr->next;
	}
	print_path(*path);
	// free_path(path);
	return (join_pwd(path, pwd));
}

void	cd(char **envi, char **params) // Regler le probleme de ..
{
	int		oldpwd;
	int		pwd;
	int		x;
	char	*home;

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
			ft_putstr(strerror(errno));
			write(1, "\n", 1);
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
	if (params[1] && params[1][0] == '.' && params[1][1] == 0)
		return ;
	free(envi[pwd]);
	if (params[1] == NULL)
		envi[pwd] = ft_strjoinf2("PWD=", ft_strdup(home));
	else if (params[1][0] == '/')
		envi[pwd] = ft_strjoinf2("PWD=", ft_strdup(params[1]));
	else
		envi[pwd] = ft_strjoinf2(&envi[oldpwd][3], ft_strjoinf2("/", ft_strdup(params[1])));
	envi[pwd] = parse_dot_dot(envi[pwd]);
}

void	pwd(char **envi, char **params)
{
	int x;

	x = 0;
	if (params[1])
	{
		ft_putstr("minishell: pwd: too many arguments\n");
		return ;
	}
	while (envi[x] && ft_strncmp(envi[x], "PWD=", 4) != 0)
		++x;
	if (envi[x])
		ft_putstr(&envi[x][4]);
	write(1, "\n", 1);
}

void	checkinput(char **envi, char **params)
{
	if (ft_strcmp(params[0], "exit") == 0) // Fini
	{
		ft_putstr("exit\n");
		if (params[1])
			ft_putstr("minishell: exit: too many arguments\n");
		freechar2ptr(params);
		freechar2ptr(envi);
		exit(0);
	}
	else if (ft_strcmp(params[0], "echo") == 0) // A terminer
		echo(params);
	else if (ft_strcmp(params[0], "env") == 0) // Fini
		env(envi, params);
	else if (ft_strcmp(params[0], "cd") == 0) // A terminer
		cd(envi, params);
	else if (ft_strcmp(params[0], "pwd") == 0) // Fini
		pwd(envi, params);
	else if (ft_strcmp(params[0], "export") == 0) // A terminer
		return ;
	else if (ft_strcmp(params[0], "unset") == 0) // A terminer
		return ;
}

char	**newenviron()
{
	char	**envi;
	int		x;

	x = 0;
	while (environ[x])
		++x;
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
			checkinput(envi, params);
		freechar2ptr(params);
	}
}
