/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plusdeplace.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpottier <hpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:45 by hpottier          #+#    #+#             */
/*   Updated: 2020/08/09 21:11:12 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		countpipes(t_list *curr)
{
	int x;

	x = 0;
	while (curr)
	{
		if (curr->type == 6)
			++x;
		else if (curr->type == 3)
			return (x);
		curr = curr->next;
	}
	return (x);
}

int		allocpipepid(int npipe, t_list *copy)
{
	if (npipe != 0)
	{
		if (((g_mshell.pipes) = (int *)malloc(sizeof(int) * npipe * 2)) == NULL)
		{
			ft_printh(2, 1, "minishell: %s", strerror(errno));
			g_mshell.ls = copy;
			g_mshell.exitcode = 2;
			return (1);
		}
		if (!((g_mshell.pidtab) = (int *)malloc(sizeof(int) * (npipe + 1))))
		{
			ft_printh(2, 1, "minishell: %s", strerror(errno));
			g_mshell.ls = copy;
			free((g_mshell.pipes));
			g_mshell.exitcode = 2;
			return (1);
		}
	}
	return (0);
}
