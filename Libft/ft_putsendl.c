/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putsendl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpottier <hpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 17:04:52 by hpottier          #+#    #+#             */
/*   Updated: 2019/12/02 18:47:21 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putsendl(const char *s)
{
	if (s != NULL)
	{
		write(1, s, ft_strlen(s));
		write(1, "\n", 1);
	}
}
