/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstprint.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 20:12:58 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/09 20:17:01 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	elemprint(t_list *lst, int bn)
{
	ft_putchar_fd('|', 2);
	ft_putstr_fd(lst->content, 2);
	ft_putchar_fd(':', 2);
	ft_putnbr_fd(lst->type, 2);
	ft_putchar_fd('-', 2);
	ft_putnbr_fd(lst->nospace, 2);
	ft_putchar_fd('-', 2);
	ft_putnbr_fd(lst->rawtext, 2);
	ft_putchar_fd('|', 2);
	if (bn)
		ft_putchar('\n');
}

void	ft_lstprint(t_list *lst)
{
	if (!lst)
		return ;
	while (lst->next)
	{
		elemprint(lst, 0);
		lst = lst->next;
	}
	elemprint(lst, 1);
}
