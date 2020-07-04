/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   n_itoa.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/04 10:20:51 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/04 14:50:19 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		getlen(int n)			//PROTECTED
{
	int		i;

	i = 1;
	if (n < 0)
		i++;
	while (n > 9 || n < -9)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

int				ft_pow(int nb, int pow)		//PROTECTED
{
	int		i;
	int		dest;

	dest = 1;
	i = 0;
	while (++i < pow)
		dest = dest * nb;
	return (dest);
}

char			*ft_itoaa(int n)		//PROTECTED
{
	char	*out;
	int		len;
	int		i;
	int		neg;
	int		indxo;

	indxo = 0;
	i = -1;
	len = getlen(n);
	neg = -1;
	if (!(out = malloc(sizeof(char) * (len + 1))))
		return (0);
	if (n < 0)
	{
		neg = 1;
		out[indxo++] = '-';
		len--;
	}
	while (++i < len)
	{
		out[indxo++] = (n / ft_pow(10, len - i) * -neg) + '0';
		n -= n / ft_pow(10, len - i) * ft_pow(10, len - i);
	}
	out[indxo] = 0;
	return (out);
}
