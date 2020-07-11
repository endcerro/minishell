/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maincreat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/11 14:44:42 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/11 18:51:37 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint(int sig)
{
	if (g_mshell.pid == 0)
	{
		g_mshell.exitcode = 1;
		ft_putstr("\b\b  \n\033[31mminishell \033[33m@>\033[0m");
		g_mshell.linestate = 2;
	}
	else
	{
		kill(g_mshell.pid, sig);
		g_mshell.pid = 0;
		g_mshell.sigswitch = 130;
		write(1, "\n", 1);
	}
}

void	sigquit(int sig)
{
	if (g_mshell.pid == 0)
	{
		g_mshell.exitcode = 0;
		ft_putstr("\b\b  \b\b");
		g_mshell.linestate = 2;
	}
	else
	{
		kill(g_mshell.pid, sig);
		g_mshell.pid = 0;
		g_mshell.sigswitch = 131;
		write(1, "\n", 1);
	}
}

int		isquote(char c)
{
	if (c == '\'' || c == '\"')
		return (c);
	return (0);
}

void	find_char(char *buff)
{
	if (buff[0] == 0)
		buff[0] = ';';
	else if (buff[0] == ';')
		buff[0] = '>';
	else if (buff[0] == '>')
		buff[0] = '<';
	else if (buff[0] == '<')
		buff[0] = '|';
	else if (buff[0] == '|')
		buff[0] = '\'';
	else if (buff[0] == '\'')
		buff[0] = '\"';
	else if (buff[0] == '\"')
		buff[0] = 0;
}
