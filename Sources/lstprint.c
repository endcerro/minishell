/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstprint.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 20:12:58 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/19 21:06:29 by edal--ce         ###   ########.fr       */
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

int		baderrorohno(void)
{
	ft_putstr_fd("minishell: cd: error retrieving current directory: getc", 2);
	ft_putstr_fd("wd: cannot access parent directories: ", 2);
	return (ft_printh(2, 1, "%s\n", strerror(errno)));
}
