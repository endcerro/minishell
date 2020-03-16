/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpottier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 13:09:14 by hpottier          #+#    #+#             */
/*   Updated: 2019/11/07 14:35:57 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t			i;
	unsigned char	*ptr;

	i = ft_strlen(s) + 1;
	ptr = NULL;
	while (i > 0)
	{
		if (s[i - 1] == (unsigned char)c)
		{
			ptr = (unsigned char *)s + i - 1;
			return ((char *)ptr);
		}
		--i;
	}
	return (NULL);
}
