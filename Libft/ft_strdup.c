/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 13:35:30 by hpottier          #+#    #+#             */
/*   Updated: 2020/08/10 18:42:01 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*ret;

	i = 0;
	if (s1 == NULL)
		return (NULL);
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
