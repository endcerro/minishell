/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 20:19:40 by hpottier          #+#    #+#             */
/*   Updated: 2020/08/08 19:04:56 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	getraw(char *str)
{
	int		i;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		deconechar(str + i);
	while (str[i])
	{
		if (ft_isspace(str[i]))
			str[i] = ' ';
		i++;
	}
	i = 0;
	while (str[i])
	{
		if (str[i] == ' ' && str[i + 1] == ' ')
			deconechar(str + i);
		else
			i++;
	}
}

void	trim_quotes(char *str)
{
	int i;

	i = 1;
	if (str[0] != '\'' && str[0] != '\"')
		return ;
	if (str[1] == 0)
	{
		str[0] = 0;
		return ;
	}
	while (str[i])
	{
		str[i - 1] = str[i];
		i++;
	}
	str[i - 2] = 0;
}

int		is_n(char *str)
{
	int i;

	i = 0;
	if (str[i++] != '-')
		return (0);
	while (str[i] == 'n')
		i++;
	if (str[i] != 0)
		return (0);
	return (1);
}

int		echo_ls(void)
{
	int		ret;
	t_list	*curr;

	ret = 0;
	curr = g_mshell.ls->next;
	// printf("IN ECHO\n");
	// ft_lstprint(g_mshell.ls);
	if (curr && is_n(curr->content))
		while (curr && is_n(curr->content))
			curr = curr->next;
	else
		ret = 1;
	while (curr && curr->type == 1)
	{
		if (curr->rawtext)
			getraw(curr->content);
		ft_putstr(curr->content);
		if (curr->next)
			write(1, " ", 1);
		curr = curr->next;
	}
	if (ret)
		write(1, "\n", 1);
	return (0);
}
