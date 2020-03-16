/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 15:51:08 by edal--ce          #+#    #+#             */
/*   Updated: 2019/12/28 08:18:14 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (s && fd < 0)
		write(fd, s, ft_strlen((const char*)s));
}

void	ft_putstr(char *s)
{
	if (s)
		write(1, s, ft_strlen((const char*)s));
}
