/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 14:08:05 by hpottier          #+#    #+#             */
/*   Updated: 2020/06/10 15:04:23 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_strs(char *str1, char *str2, int mode)
{
	if (mode == 1)
		free(str1);
	else if (mode == 2)
		free(str2);
	else if (mode == 3)
	{
		free(str1);
		free(str2);
	}
}

char	*ft_strjoinf1(char *s1, char const *s2)
{
	char	*ret;
	int		i;
	int		j;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	if (!(ret = (char *)malloc(sizeof(char) *
							(ft_strlen(s1) + ft_strlen(s2) + 1))))
	{
		free_strs(s1, (char *)s2, 1);
		return (NULL);
	}
	i = -1;
	j = 0;
	while (s1[++i])
		ret[i] = s1[i];
	while (s2[j])
	{
		ret[i] = s2[j];
		++i;
		++j;
	}
	free(s1);
	ret[i] = 0;
	return (ret);
}

char	*ft_strjoinf2(char const *s1, char *s2)
{
	char	*ret;
	int		i;
	int		j;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	if (!(ret = (char *)malloc(sizeof(char) *
							(ft_strlen(s1) + ft_strlen(s2) + 1))))
	{
		free_strs((char *)s1, s2, 2);
		return (NULL);
	}
	i = -1;
	j = 0;
	while (s1[++i])
		ret[i] = s1[i];
	while (s2[j])
		ret[i++] = s2[j++];
	free(s2);
	ret[i] = 0;
	return (ret);
}

char	*ft_strjoinft(char *s1, char *s2)
{
	char	*ret;
	int		i;
	int		j;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	if (!(ret = (char *)malloc(sizeof(char) *
							(ft_strlen(s1) + ft_strlen(s2) + 1))))
	{
		free_strs(s1, s2, 3);
		return (NULL);
	}
	i = -1;
	j = 0;
	while (s1[++i])
		ret[i] = s1[i];
	while (s2[j])
		ret[i++] = s2[j++];
	ret[i] = 0;
	free(s1);
	free(s2);
	return (ret);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ret;
	int		i;
	int		j;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	if (!(ret = (char *)malloc(sizeof(char) *
							(ft_strlen(s1) + ft_strlen(s2) + 1))))
		return (NULL);
	i = -1;
	j = 0;
	while (s1[++i])
		ret[i] = s1[i];
	while (s2[j])
		ret[i++] = s2[j++];
	ret[i] = 0;
	return (ret);
}
