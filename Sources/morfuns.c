/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   morfuns.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 16:49:12 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/09 16:49:45 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		correctlst(t_list *lst)
{
	while (lst && lst->content)
	{
		trim_quotes(lst->content);
		lst = lst->next;
	}
	return (0);
}