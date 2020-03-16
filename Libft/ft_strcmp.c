/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strncmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpottier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 15:04:33 by hpottier          #+#    #+#             */
/*   Updated: 2019/11/04 18:08:43 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t len1;
	size_t len2;

	len1 = ft_strlen(s1) + 1;
	len2 = ft_strlen(s2) + 1;
	return (ft_memcmp(s1, s2, (len1 < len2) ? len1 : len2));
}
