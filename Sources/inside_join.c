/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inside_join.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/16 23:50:31 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/10 00:31:31 by edal--ce         ###   ########.fr       */
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

void	iniside_join_loop(char *base, char *add, char *out, int exit)
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
			while (++j && base[i] && (ft_isalnum(base[i])
			|| (exit && base[i] == '?')))
			{
				i++;
				exit = 0;
			}
		}
		else
			out[p++] = base[i++];
	}
	out[p] = 0;
}

void escape_nested_var(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			str[i] = -2;
		i++;
	}
}

char	*inside_join(char *base, char *add, int mode, int exit)
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
	if (mode == 1 )
	{
		mode = 3;
		add = ft_strdup(add);			
		escape_nested_var(add);
	}
	iniside_join_loop(base, add, out, exit);
	select_free(base, add, mode);
	return (out);
}
