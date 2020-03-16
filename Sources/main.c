/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/16 20:40:33 by edal--ce          #+#    #+#             */
/*   Updated: 2020/03/16 23:53:13 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	prompt(char **line)
{
	ft_putstr("minishell @>");
	get_next_line(0, line);
}

void	freeparams(char **params)
{
	int i;

	i = 0;
	while (params[i])
	{
		free(params[i]);
		++i;
	}
	free(params);
}

void	echo(char **params)	// Ne gere ni les escape ni les quotes
{							// Il y a aussi des problemes avec les espaces lie au split
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

void	env(char **envp, char **params)
{
	int x;

	x = 0;
	if (params[1])
		ft_putstr("minishell: env: too many arguments\n");
	else
	{
		while (envp[x])
		{
			ft_putstr(envp[x]);
			write(1, "\n", 1);
			++x;
		}
	}
}

void	cd(char **params) // Il faut changer PWD et OLDPWD dans env
{
	if (params[1] && params[2] != 0)
	{
		ft_putstr("minishell: cd: too many arguments\n");
		return ;
	}
	if (chdir(params[1]) == -1)
	{
		ft_putstr(strerror(errno));
		write(1, "\n", 1);
	}
}

void	pwd(char **envp, char **params)
{
	int x;

	x = 0;
	if (params[1])
	{
		ft_putstr("minishell: pwd: too many arguments\n");
		return ;
	}
	while (ft_strncmp(envp[x], "PWD=", 4) != 0)
		++x;
	ft_putstr(&envp[x][4]);
	write(1, "\n", 1);
}

void	checkinput(char **envp, char **params)
{
	if (ft_strcmp(params[0], "exit") == 0) // Fini
	{
		ft_putstr("exit\n");
		if (params[1])
			ft_putstr("minishell: exit: too many arguments\n");
		freeparams(params);
		exit(0);
	}
	if (ft_strcmp(params[0], "echo") == 0) // A terminer
	{
		echo(params);
		return ;
	}
	if (ft_strcmp(params[0], "env") == 0) // Fini
	{
		env(envp, params);
		return ;
	}
	if (ft_strcmp(params[0], "cd") == 0) // A terminer
	{
		cd(params);
		return ;
	}
	if (ft_strcmp(params[0], "pwd") == 0) // Fini
	{
		pwd(envp, params);
		return ;
	}
	if (ft_strcmp(params[0], "export") == 0) // A terminer
	{
		return ;
	}
	if (ft_strcmp(params[0], "unset") == 0) // A terminer
	{
		return ;
	}
}

int		main(int ac, char **av, char **envp)
{
	char *line;
	char **params;

	(void)ac;
	(void)av;
	(void)envp;
	while (1)
	{
		prompt(&line);
		if (line == NULL && *line == 0)
			break ;
		if (!(params = get_blocks(line)))	// Ce split est naze il faut le changer
			return (0);							// Il ne gere pas les quotes et les whitespaces
		free(line);
		if (*params)
			checkinput(envp, params);
		freeparams(params);
	}
	return (0);
}
