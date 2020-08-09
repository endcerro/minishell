/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commandslst3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 19:12:57 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/09 19:17:59 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list 		*rdout(t_list **curr, t_list **prev)
{
	int fd;

	curr->type = -curr->type;
	if (curr->next->next && !islastrdir(curr, -2))
	{
		fd = open(curr->next->content, (curr->type == 2 ? : O_TRUNC | O_APPEND)
		| O_WRONLY | O_CREAT, 0644);
		if (fd == -1)
		{
			ft_printh(2, 1, "minishell: %s: no such file or directory\n", curr->next->content);
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

int 		trim_rdir(t_list *lst)
{
	t_list *curr;
	t_list *prev;
	int fd;

	curr = lst;
	prev = 0;
	ft_lstprint(lst);
	while (curr && curr->type != 3)
	{
		while (curr && curr->type != 6)
		{
			if (curr->type == -2 )
			{
				// curr->type = 2;
				// if (curr->next->next && !islastrdir(curr, -2))
				// {
				// 	fd = open(curr->next->content, O_APPEND | O_TRUNC | O_WRONLY | O_CREAT
				// , 0644);
				// 	if (fd == -1)
				// 	{
				// 		ft_printh(2, 1, "minishell: %s: no such file or directory\n", curr->next->content);
				// 		return ((g_mshell.exitcode = 1));
				// 	}
				// 	close(fd);
				// 	prev->next = curr->next->next;
				// 	ft_lstdelone(curr->next);
				// 	ft_lstdelone(curr);
				// 	curr = prev;
				// 	continue ;
				// }
			}
			else if (curr->type == -4)
			{
				curr->type = 4;
				if (curr->next->next && !islastrdir(curr, -4))
				{
					fd = open(curr->next->content, O_APPEND | O_WRONLY | O_CREAT
				, 0644);
					if (fd == -1)
					{
						ft_printh(2, 1, "minishell: %s: no such file or directory\n", curr->next->content);
						return ((g_mshell.exitcode = 1));
					}
					close(fd);
					prev->next = curr->next->next;
					ft_lstdelone(curr->next);
					ft_lstdelone(curr);
					curr = prev;
					continue ;
				}
			}
			else if (curr->type == -5)
			{
				curr->type = 5;
				if (curr->next->next && !islastrdir(curr, -5))
				{
					int fd;

					fd = open(curr->next->content, O_RDONLY);
					if (fd == -1)
					{
						ft_printh(2, 1, "minishell: %s: no such file or directory\n", curr->next->content);
						return ((g_mshell.exitcode = 1));
					}
					close(fd);
					prev->next = curr->next->next;
					ft_lstdelone(curr->next);
					ft_lstdelone(curr);
					curr = prev;
					continue ;
				}
			}
			prev = curr;
			curr = curr->next;
		}
		prev = curr;
		curr = (curr) ? curr->next : 0;
	}
	g_mshell.ls = lst;
	ft_lstprint(g_mshell.ls);
	return (0);
}