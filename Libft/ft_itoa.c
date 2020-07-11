/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 15:32:49 by hpottier          #+#    #+#             */
/*   Updated: 2020/07/11 14:34:01 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_count_recurs(unsigned int nbr)
{
	if (nbr)
		return (ft_count_recurs(nbr / 10) + 1);
	return (0);
}

static char		*ft_alloc(int size, int neg)
{
	char *ret;

	if (size == 0)
	{
		if (!(ret = (char *)malloc(sizeof(char) * 2)))
			return (NULL);
		ret[0] = '0';
		ret[1] = 0;
		return (ret);
	}
	if (!(ret = (char *)malloc((size + neg) * sizeof(char))))
		return (NULL);
	if (neg)
		ret[0] = '-';
	ret[size + neg] = 0;
	return (ret);
}

static void		ft_recurs(char *ret, unsigned int nbr, int i)
{
	if (nbr)
	{
		ft_recurs(ret, nbr / 10, i - 1);
		ret[i] = (nbr % 10) + 48;
	}
}

char			*ft_itoa(int nb)
{
	unsigned int	nbr;
	size_t			size;
	int				neg;
	char			*ret;

	neg = 0;
	if (nb < 0 && nb == -2147483648)
	{
		neg = 1;
		nbr = 2147483648;
	}
	else if (nb < 0 && nb != -2147483648)
	{
		neg = 1;
		nbr = (unsigned int)(-1 * nb);
	}
	else
		nbr = (unsigned int)nb;
	size = ft_count_recurs(nbr);
	if (!(ret = ft_alloc(size, neg)))
		return (NULL);
	ft_recurs(ret, nbr, size + neg - 1);
	return (ret);
}
