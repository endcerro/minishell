/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 20:19:40 by hpottier          #+#    #+#             */
/*   Updated: 2020/07/11 14:26:05 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int		echo_ls(void)
{
	int		ret;
	t_list	*curr;

	ret = 0;
	curr = g_mshell.ls->next;
	if (curr && ft_strcmp(curr->content, "-n") == 0)
		while (curr && ft_strcmp(curr->content, "-n") == 0)
			curr = curr->next;
	else
		ret = 1;
	while (curr && curr->type == 1)
	{
		ft_putstr(curr->content);
		if (curr->next)
			write(1, " ", 1);
		curr = curr->next;
	}
	if (ret)
		write(1, "\n", 1);
	return (0);
}
