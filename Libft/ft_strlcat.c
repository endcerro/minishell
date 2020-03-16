/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpottier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 23:03:35 by hpottier          #+#    #+#             */
/*   Updated: 2019/11/04 19:43:24 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t i;
	size_t h;
	size_t srclen;

	srclen = ft_strlen(src);
	i = ft_strlen(dest);
	h = 0;
	if (size <= i)
		srclen = srclen + size;
	else
		srclen = srclen + i;
	if (size > i)
		while (src[h] && i + 1 < size)
		{
			dest[i] = src[h];
			h++;
			i++;
		}
	dest[i] = 0;
	return (srclen);
}
