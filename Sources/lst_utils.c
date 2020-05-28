/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 15:29:38 by edal--ce          #+#    #+#             */
/*   Updated: 2020/05/28 12:15:14 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_lstprint(t_list *lst)
{
	if (lst)
	{
		 while (lst->next)
		{
		 	ft_putchar('|');
		 	ft_putstr(lst->content);
		 	ft_putchar(':');
		 	ft_putnbr(lst->type);
			ft_putchar('|');
			lst = lst->next;
		}
		ft_putchar('|'); 
		ft_putstr(lst->content);
		ft_putchar(':');
		ft_putnbr(lst->type);
		ft_putchar('|');
	}
	ft_putchar('\n');
}

void	ft_lstdelone(t_list *lst)
{
	free(lst->content);
	free(lst);
}

void	ft_lstclear(t_list **lst)
{
	t_list *cache;
	t_list *cache2;

	cache = *lst;
	while (cache->next)
	{
		cache2 = cache->next;
		ft_lstdelone(cache);
		cache = cache2;
	}
	ft_lstdelone(cache);
	*lst = 0;
}

t_list	*ft_lstnew(char *content)
{
	t_list *out;

	if (!(out = malloc(sizeof(t_list))))
		return (0);
	out->content = content;
	out->next = 0;
	out->type = 1;
	return (out);
}

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list *tmp;

	if (alst && new)
	{
		tmp = *alst;
		if (tmp)
		{
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
		else
			*alst = new;
	}
}

void	ft_lstadd_front(t_list **alst, t_list *new)
{
	if (alst && new)
	{
		new->next = *alst;
		*alst = new;
	}
}

void	ft_lstadd_next(t_list *alst, t_list *new)
{
	t_list *copy;

	copy = alst->next;

	alst->next = new;
	new->next = copy;
}
