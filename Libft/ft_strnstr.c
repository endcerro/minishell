/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpottier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 15:24:13 by hpottier          #+#    #+#             */
/*   Updated: 2019/11/04 18:30:08 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	size_t i;
	size_t h;
	size_t g;

	i = 0;
	if (to_find[0] == 0)
		return ((char *)str);
	while (str[i] && i < len)
	{
		h = 0;
		g = i;
		if (str[i] == to_find[0])
			while (str[g] == to_find[h])
			{
				if (to_find[h + 1] == 0 && g < len)
					return ((char *)&str[i]);
				++h;
				++g;
			}
		++i;
	}
	return (NULL);
}
