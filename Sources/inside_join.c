/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inside_join.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/16 23:50:31 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/08 15:49:53 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	select_free(char *str1, char *str, int mode)
{
	if (mode == 1)
		free(str1);
	else if (mode == 2)
		free(str);
	else
	{
		free(str1);
		free(str);
	}
}

void	iniside_join_loop(char *base, char *add, char *out)
{
	int i;
	int j;
	int p;

	j = 0;
	p = 0;
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
}

char	*inside_join(char *base, char *add, int mode)	//MALLOC PROC OK
{
	char	*out;

	if (add != 0)
	{
		if (!(out = malloc(sizeof(char) *
				(ft_strlen(base) + ft_strlen(add) + 1))))
		{
			select_free(base, add, mode);
			return (0);
		}

	}
	else
		out = ft_strdup(base);
	if (out == 0)
	{
		select_free(base, add, mode);
		return (0);
	}
	iniside_join_loop(base, add, out);
	select_free(base, add, mode);
	return (out);
}
