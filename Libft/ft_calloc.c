/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 16:06:37 by hpottier          #+#    #+#             */
/*   Updated: 2020/07/11 14:34:02 by edal--ce         ###   ########.fr       */
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
