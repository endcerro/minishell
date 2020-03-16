/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpottier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 13:35:30 by hpottier          #+#    #+#             */
/*   Updated: 2019/12/30 17:42:47 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*ret;

	i = 0;
	while (s1[i])
		++i;
	ret = (char *)malloc((i + 1) * sizeof(char));
	if (ret == NULL)
		return (NULL);
	while (i > -1)
	{
		ret[i] = s1[i];
		--i;
	}
	return (ret);
}
