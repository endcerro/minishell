/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printh.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/10 14:05:33 by hpottier          #+#    #+#             */
/*   Updated: 2020/07/11 14:33:57 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static ssize_t	strccpy_perc(const char **src, char **dst)
{
	ssize_t	i;
	ssize_t	size;

	size = 0;
	while (*(*src + size) && *(*src + size) != '%')
		++size;
	if (!(*dst = (char *)malloc(sizeof(char) * (size + 1))))
		return (-1);
	i = 0;
	while (i < size)
	{
		*(*dst + i) = **src;
		++*src;
		++i;
	}
	*(*dst + i) = 0;
	return (size);
}

static void		checkarg(const char **format, va_list *ap, int fd)
{
	char *str;

	if (*(*format + 1) == '%')
		write(1, "%", 1);
	else
	{
		str = va_arg(*ap, char *);
		if (str == NULL)
			ft_putstr_fd("(null)", fd);
		else
			ft_putstr_fd(str, fd);
	}
	*format += 2;
}

static void		putformat(const char *format, va_list *ap, int fd)
{
	char *str;

	while (*format)
	{
		if (strccpy_perc(&format, &str) == -1)
			return ;
		else
		{
			ft_putstr_fd(str, 2);
			free(str);
		}
		if (*format == '%')
			checkarg(&format, ap, fd);
	}
}

int				ft_printh(int fd, int ret, const char *format, ...)
{
	va_list ap;

	if (*format != 0)
	{
		va_start(ap, format);
		putformat(format, &ap, fd);
		va_end(ap);
	}
	return (ret);
}
