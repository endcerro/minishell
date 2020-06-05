/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 19:21:47 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/05 19:03:00 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void trimquotes(char *str)
{
	int i;
	int j;

	j = 0;
	i = 0;

	while(str[i])
	{
		if(str[i] == '\'' || str[i] == '\"')
		{
			j = i;
			while(str[++j])
			{
				str[j - 1] = str[j];
			}
			str[j - 1] = 0;
		}
		i++;
	}
	printf("Trimmed = %s\n",str );
}

int check_valid_export(t_list *curr) //TODO
{
	int i;

	i = 0;

	printf("check_valid_export for %s\n",curr->content);
	trimquotes(curr->content);
	// if(ft_strchr)
	if (ft_isdigit(curr->content[0]))
		return (1);
	while (curr->content[i])
	{
		if(curr->content[i] == '\n')
			return (1);
		i++;
	}

	return (0);
}
