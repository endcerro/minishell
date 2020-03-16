/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpottier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 12:01:16 by hpottier          #+#    #+#             */
/*   Updated: 2019/11/12 17:06:14 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t i;

	i = 0;
	if (src == dst)
		return (dst);
	if (src + len > dst && src + len < dst + len)
		while (len > 0)
		{
			*((unsigned char *)dst + len - 1) = *((unsigned char *)src
												+ len - 1);
			--len;
		}
	else
		while (i < len)
		{
			*((unsigned char *)dst + i) = *((unsigned char *)src + i);
			++i;
		}
	return (dst);
}
