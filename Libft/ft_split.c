/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 16:55:09 by hpottier          #+#    #+#             */
/*   Updated: 2020/06/04 19:15:32 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int			count_word(char const *str, char c)
{
	int i;
	int count;

	i = 0;
	count = 1;
	while (str[i] && str[i] == c)
		++i;
	while (str[i])
	{
		if (str[i] == c && str[i + 1] && str[i + 1] == c)
		{
			while (str[i] && str[i] == c)
				++i;
			if (str[i])
				++count;
		}
		++i;
	}
	return (count + 1);
}

static char			*ret_word(char const *str, char c)
{
	char	*ret;
	size_t	i;

	i = 0;
	while (str[i] && str[i] != c)
		++i;
	if (!(ret = (char *)malloc(sizeof(char) * (i + 1))))
		return (NULL);
	ret[i] = 0;
	i = 0;
	while (str[i] && str[i] != c)
	{
		ret[i] = str[i];
		++i;
	}
	return (ret);
}

char				**ft_split(char const *str, char c)
{
	char	**tab;
	int		i;
	int		g;

	if (str == NULL)
		return (NULL);
	if (!(tab = (char **)malloc(sizeof(char *) * count_word(str, c))))
		return (NULL);
	i = 0;
	g = 0;
	while (str[g])
	{
		while (str[g] && str[g] == c)
			++g;
		if (str[g])
		{
			tab[i] = ret_word(&str[g], c);
			i++;
			while (str[g] && !(str[g] == c))
				++g;
		}
	}
	tab[i] = 0;
	return (tab);
}
