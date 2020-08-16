/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:49 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/16 16:36:06 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*add_filler(char **fill)
{
	int		i;
	char	*out;

	out = 0;
	i = 0;
	if (fill && fill[i])
	{
		if ((out = ft_strdup(fill[i])) == NULL)
			return ((char *)(long)freechar2ptr(fill, 0));
		while (fill[++i])
		{
			if ((out = ft_strjoinf1(out, "\n")) == NULL)
				break ;
			if ((out = ft_strjoinf1(out, fill[i])) == NULL)
				break ;
		}
	}
	return (out);
}

void 	check_end_pipe(char *line, char *cpt)
{
	int i;

	i = -1;
	while (line && line[++i])
	{
		if (line[i] == '|')
		{
			fspace = 1;
			++i;
			while(ft_isspace(line[i]))
				i++;
			if (line[i] == 0)
			{
				cpt[3] = 1;
				return ;
			}
			else
				cpt[3] = 0;
		}
		else
			cpt[3] = 0;
	}
}

char	*check_finished_lst(char *line, int *err)
{
	char	cpt[4];
	char	**fill;
	char	*out;

	fill = 0;
	out = 0;
	ft_bzero(cpt, 4);
	escape_chars(line, 0, 0, cpt);
	check_end_pipe(line, cpt);
	// ft_putstr("MADE IT");
	parse_qts(line, cpt);
	if (cpt[0] % 2 || cpt[1] % 2 || cpt[2] || cpt[3])
		if ((fill = getfiller(0, cpt, err)) == NULL)
			return (NULL);
	if ((out = add_filler(fill)) == NULL)
		return (NULL);
	freechar2ptr(fill, 0);
	return (out);
}
