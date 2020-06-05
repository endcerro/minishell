/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 19:04:24 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/05 19:15:03 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		wordlen(char *str)
{
	int i;

	i = -1;
	while (str[++i])
	{
		if (!ft_isalnum(str[i]))
			return (i);
	}
	return (i);
}

int		parti(char **tab, int lo, int hi)
{
	char	*pivot;
	char	*tmp;
	int		i;
	int		j;

	pivot = tab[hi];
	i = (lo - 1);
	j = lo;
	while (j <= hi)
	{
		if (ft_strcmp(tab[j], pivot) < 0)
		{
			++i;
			tmp = tab[i];
			tab[i] = tab[j];
			tab[j] = tmp;
		}
		++j;
	}
	tmp = tab[i + 1];
	tab[i + 1] = tab[hi];
	tab[hi] = tmp;
	return (i + 1);
}

void	quicks(char **tab, int lo, int hi)
{
	int pi;

	if (lo < hi)
	{
		pi = parti(tab, lo, hi);
		quicks(tab, lo, pi - 1);
		quicks(tab, pi + 1, hi);
	}
}

void	freechar2ptr(char **ptr)
{
	int i;

	i = 0;
	if (ptr == 0)
		return ;
	while (ptr[i])
	{
		free(ptr[i]);
		++i;
	}
	free(ptr);
}
