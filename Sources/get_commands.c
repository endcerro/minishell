/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/16 23:50:31 by edal--ce          #+#    #+#             */
/*   Updated: 2020/05/26 16:10:37 by edal--ce         ###   ########.fr       */
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
			// printf("block = %s \n",line + i );
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