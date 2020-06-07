/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 14:22:32 by hpottier          #+#    #+#             */
/*   Updated: 2020/06/07 17:35:12 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*ret;

	if (s == NULL)
		return (NULL);
	if (!(ret = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	if (ft_strlen(s) < start)
	{
		ret[0] = 0;
		return (ret);
	}
	i = (size_t)start;
	j = 0;
	while (j < len && s[i])
	{
		ret[j] = s[i];
		++i;
		++j;
	}
	ret[j] = 0;
	return (ret);
}

char	*ft_substrf1(char *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*ret;

	if (s == NULL)
		return (NULL);
	if (!(ret = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	if (ft_strlen(s) < start)
	{
		ret[0] = 0;
		return (ret);
	}
	i = (size_t)start;
	j = 0;
	while (j < len && s[i])
	{
		ret[j] = s[i];
		++i;
		++j;
	}
	ret[j] = 0;
	free(s);
	return (ret);
}
