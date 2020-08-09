/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commandslst3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 19:12:57 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/09 19:41:49 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list		*rdout(t_list *curr, t_list *prev)
{
	int fd;

	curr->type = -curr->type;
	if (curr->next->next && !islastrdir(curr, (curr->type == 2 ? -2 : -4)))
	{
		fd = open(curr->next->content, (curr->type == 2 ? O_TRUNC : O_APPEND)
		| O_WRONLY | O_CREAT, 0644);
		if (fd == -1)
		{
			ft_printh(2, 1, "minishell: %s: no such file or directory\n",
				curr->next->content);
			return ((t_list *)(long)(g_mshell.exitcode = 1));
		}
		close(fd);
		prev->next = curr->next->next;
		ft_lstdelone(curr->next);
		ft_lstdelone(curr);
		curr = prev;
		return (curr);
	}
	return ((t_list *)(long)-1);
}

t_list		*rdin(t_list *curr, t_list *prev)
{
	int fd;

	curr->type = -curr->type;
	if (curr->next->next && !islastrdir(curr, -5))
	{
		fd = open(curr->next->content, O_RDONLY);
		if (fd == -1)
		{
			ft_printh(2, 1, "minishell: %s: no such file or directory\n",
				curr->next->content);
			return ((t_list *)(long)(g_mshell.exitcode = 1));
		}
		close(fd);
		prev->next = curr->next->next;
		ft_lstdelone(curr->next);
		ft_lstdelone(curr);
		curr = prev;
		return (curr);
	}
	return ((t_list*)(long)-1);
}

int			trim_rdir(t_list *curr)
{
	t_list *prev;
	t_list *tmp;

	while (curr && curr->type != 3)
	{
		while (curr && curr->type != 6)
		{
			if (curr->type == -2 || curr->type == -4 || curr->type == -5)
			{
				if (curr->type == -5)
					tmp = rdin(curr, prev);
				else
					tmp = rdout(curr, prev);
				if ((int)tmp == 1)
					return (1);
				else if ((int)tmp != -1 && (curr = tmp))
					continue;
			}
			prev = curr;
			curr = curr->next;
		}
		prev = curr;
		curr = curr ? curr->next : 0;
	}
	return (0);
}
