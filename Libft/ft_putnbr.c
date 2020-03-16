/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpottier <hpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 17:03:21 by hpottier          #+#    #+#             */
/*   Updated: 2019/12/02 17:04:25 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_putnbr_recurs(int n)
{
	if (n)
	{
		ft_putnbr_recurs(n / 10);
		ft_putchar(n % 10 + 48);
	}
}

void		ft_putnbr(int n)
{
	if (n == -2147483648)
	{
		n = 147483648;
		ft_putchar('-');
		ft_putchar('2');
	}
	if (n < 0)
	{
		n *= -1;
		ft_putchar('-');
	}
	if (n)
		ft_putnbr_recurs(n);
	else
		ft_putchar('0');
}
