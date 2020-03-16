/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpottier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 16:06:37 by hpottier          #+#    #+#             */
/*   Updated: 2019/11/05 10:57:27 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	i;

	i = size * count;
	if (!(ptr = malloc(i)))
		return (NULL);
	if (i == 0)
		return (ptr);
	while (i > 0)
	{
		--i;
		*((unsigned char *)ptr + i) = 0;
	}
	return (ptr);
}
