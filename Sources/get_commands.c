/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/16 23:50:31 by edal--ce          #+#    #+#             */
/*   Updated: 2020/04/26 22:48:33 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_blocks(char *line)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (line[i] && ft_isspace(line[i]))
		++i;
	while (line[i])
	{
		if(!ft_isspace(line[i]))
		{
			count++;
			if(line[i++] == '"')
				while(line[i] && line[i] != '"')
					++i;
			else
				while(line[i] && !ft_isspace(line[i]))
					++i;
		}
		else
			++i;
	}
	return (count);
}

char	*get_word(char *line, int *p)
{
	size_t	len;
	int		cp;

	cp = *p;
	len = 0;
	if (line[*p] == '"' && ++(*p))
	{
		while (line[*p] && line[*p] != '"' && ++len)
			++(*p);
		++(*p);
		return(ft_substr(line, cp, len + 2));
	}
	else if (line[*p] == '\'' && ++(*p))
	{
		while (line[*p] && line[*p] != '\'' && ++len)
			++(*p);
		++(*p);
		return(ft_substr(line, cp, len + 2));
	}
	else
		while (line[*p] && !ft_isspace(line[*p]) && ++len)
			++(*p);
	return(ft_substr(line, cp, len));
}

char	**get_blocks(char *line)
{
	int 	p;
	int 	i;
	char 	**out;
	
	i = 0;
	p = 0;

	if(!(out = (char **)malloc(sizeof(char *) * (count_blocks(line) + 1))))
		return 0;
	while (i < ft_strlen(line))
	{
		if (ft_isspace(line[i]))
			++i;
		else
			if ((out[p++] = get_word(line, &i)) == NULL)
				return (out);
	}
	out[p] = 0;
	return (out);
}
