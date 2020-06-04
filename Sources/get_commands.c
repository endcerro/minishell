/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/16 23:50:31 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/04 19:37:30 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*inside_join(char *base, char *add)
{
	int		i;
	int		j;
	int		p;
	char	*out;

	if (add != 0)
	{
		if (!(out = malloc(sizeof(char) *
				(ft_strlen(base) + ft_strlen(add) + 1))))
			return (0);
	}
	else
		out = ft_strdup(base);
	p = 0;
	j = 0;
	i = 0;
	while (base[i])
	{
		if (j == 0 && base[i] == '$')
		{
			while (add && add[j])
				out[p++] = add[j++];
			i++;
			while (++j && base[i] && !ft_isspace(base[i]))
				i++;
		}
		else
			out[p++] = base[i++];
	}
	out[p] = 0;
	return (out);
}
