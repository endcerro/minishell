/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 15:29:01 by hpottier          #+#    #+#             */
/*   Updated: 2020/07/11 14:34:11 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	long	ret;
	size_t	i;
	long	neg;

	ret = 0;
	i = 0;
	neg = 1;
	while (str[i] && ft_isspace(str[i]))
		++i;
	if (str[i] == '-')
		neg = -1;
	if (str[i] == '+' || str[i] == '-')
		++i;
	while (str[i] && ft_isdigit(str[i]))
	{
		ret = (ret * 10) + (str[i] - 48);
		++i;
	}
	return ((long)(ret * neg));
}
