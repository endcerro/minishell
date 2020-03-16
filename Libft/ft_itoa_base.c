/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpottier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 18:24:08 by hpottier          #+#    #+#             */
/*   Updated: 2019/12/11 18:03:06 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_check_base(char *base)
{
	int	lenbase;
	int	i;

	lenbase = -1;
	while (base[++lenbase])
	{
		i = -1;
		if (base[lenbase] == '+' || base[lenbase] == '-' ||
			base[lenbase] == '\t' || base[lenbase] == '\n' ||
			base[lenbase] == '\v' || base[lenbase] == '\f' ||
			base[lenbase] == '\r' || base[lenbase] == ' ')
			return (0);
		while (base[++i])
			if (lenbase != i)
				if (base[lenbase] == base[i])
					return (0);
	}
	return (lenbase);
}

static int		ft_count_recurs(unsigned int nbr, int lenbase)
{
	if (nbr)
		return (ft_count_recurs(nbr / lenbase, lenbase) + 1);
	return (0);
}

static char		*ft_alloc(int size, int neg)
{
	char *ret;

	if (!(ret = (char *)malloc((size + neg) * sizeof(char))))
		return (NULL);
	if (neg)
		ret[0] = '-';
	ret[size + neg] = 0;
	return (ret);
}

static void		ft_recurs(char *ret, unsigned int nbr, int i, char *base)
{
	if (nbr)
	{
		ft_recurs(ret, nbr / ft_check_base(base), i - 1, base);
		ret[i] = base[nbr % ft_check_base(base)];
	}
}

char			*ft_itoa_base(int nb, char *base)
{
	int				lenbase;
	unsigned int	nbr;
	int				neg;
	char			*ret;

	if ((lenbase = ft_check_base(base)) < 2)
		return (NULL);
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
	if (!(ret = ft_alloc(ft_count_recurs(nbr, lenbase), neg)))
		return (NULL);
	ft_recurs(ret, nbr, ft_count_recurs(nbr, lenbase) + neg - 1, base);
	return (ret);
}
