/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rdirstuff.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpottier <hpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:45 by hpottier          #+#    #+#             */
/*   Updated: 2020/08/09 21:11:12 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_rdir(t_list **lst)
{
	t_list *prev;
	t_list *copy;
	t_list *curr;
	t_list *tmp;

	curr = *lst;
	copy = *lst;
	prev = *lst;
	while (*lst)
	{
		if ((*lst)->type == -1)
		{
			if ((*lst) == copy)
			{
				tmp = (*lst)->next;
				ft_lstdelone((*lst));
				lst = &tmp;
				continue ;
			}
			prev->next = (*lst)->next;
			ft_lstdelone((*lst));
			(*lst) = prev;
		}
		(*lst) = (*lst)->next;
	}
}

void	addlstendblock(t_list *lst, t_list *block)
{
	t_list *prev;

	prev = NULL;
	block->type = -(block->type);
	while (lst)
	{
		if (lst->next && (lst->next->type == 3 || lst->next->type == 6))
		{
			block->next->next = lst->next;
			lst->next = block;
			return ;
		}
		prev = lst;
		lst = lst->next;
	}
	prev->next = block;
}

void	correct_rdir2(t_list **block, t_list **curr, t_list **newlst)
{
	(*block) = (*curr);
	(*curr) = NULL;
	if ((*block)->next)
	{
		(*curr) = (*block)->next->next;
		(*block)->next->next = NULL;
	}
	(*newlst) = (*curr);
	addlstendblock((*curr), (*block));
	(*curr) = (*newlst);
}

t_list	*correct_rdir(t_list *lst, t_list *curr, t_list *newlst, t_list *pr)
{
	t_list *block;

	while (curr)
	{
		if ((curr->type == 2 || curr->type == 4 || curr->type == 5) && pr)
		{
			block = curr;
			pr->next = (block->next) ? block->next->next : NULL;
			if (block->next)
				block->next->next = NULL;
			addlstendblock(pr, block);
			curr = lst;
		}
		else if ((curr->type == 2 || curr->type == 4 || curr->type == 5)
				&& pr == NULL)
		{
			correct_rdir2(&block, &curr, &newlst);
			continue ;
		}
		pr = curr;
		curr = curr->next;
	}
	g_mshell.ls = newlst;
	return (0);
}

int		islastrdir(t_list *lst, int type)
{
	lst = lst->next;
	while (lst && lst->type != 3 && lst->type != 6)
	{
		if ((type == -5 && lst->type == type)
			|| (type != -5 && (lst->type == -4 || lst->type == -2)))
			return (0);
		lst = lst->next;
	}
	return (1);
}
