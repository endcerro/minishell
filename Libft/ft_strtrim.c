/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpottier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 14:48:02 by hpottier          #+#    #+#             */
/*   Updated: 2019/12/01 21:48:01 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		check_set(char const c, char const *set)
{
	size_t i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		++i;
	}
	return (0);
}

static int		check_fin(size_t j, char const *s1, char const *set)
{
	while (s1[j] && check_set(s1[j], set))
		++j;
	if (s1[j] == 0)
		return (1);
	return (0);
}

static size_t	count_char(char const *s1, char const *set)
{
	size_t i;
	size_t ret;

	i = 0;
	ret = 0;
	while (s1[i] && check_set(s1[i], set))
		++i;
	while (s1[i])
	{
		if (check_set(s1[i], set) && check_fin(i, s1, set))
			return (ret);
		++i;
		++ret;
	}
	return (ret);
}

char			*ft_strtrim(char const *s1, char const *set)
{
	char	*ret;
	size_t	i;
	size_t	g;

	if (s1 == NULL)
		return (NULL);
	if (!(ret = (char *)malloc(sizeof(char) * (count_char(s1, set) + 1))))
		return (NULL);
	i = 0;
	g = 0;
	while (s1[i] && check_set(s1[i], set))
		++i;
	while (s1[i])
	{
		if (check_set(s1[i], set) && check_fin(i, s1, set))
		{
			ret[g] = 0;
			return (ret);
		}
		ret[g] = s1[i];
		++i;
		++g;
	}
	ret[g] = 0;
	return (ret);
}
