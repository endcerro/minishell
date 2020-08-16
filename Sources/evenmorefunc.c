/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evenmorefunc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 19:45:03 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/16 17:36:57 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		exportstuffter(t_list *curr, char *tmp)
{
	curr->content = ft_strjoinf1(curr->content, "=");
	if (curr->content == 0)
		return (1);
	curr->content = ft_strjoinf1(curr->content, tmp);
	if (curr->content == 0)
		return (1);
	return (0);
}

int		expand_t(char **str)
{
	char *home;

	if (*str && (*str)[0] == '~' && (*str)[1] == 0)
	{
		free(*str);
		*str = NULL;
		if ((home = rethomedir()) == NULL)
			return (ft_printh(2, 1, "minishell: $HOME not set\n"));
		*str = ft_strdup(home);
		if (*str == 0)
			return (ft_printh(2, 1, "minishell: %s\n", strerror(errno)));
	}
	return (0);
}

void	escape_lst(t_list *curr)
{
	while (curr && curr->type == 1)
	{
		escape_chars(curr->content, -1, 0, 0);
		curr = curr->next;
	}
}
