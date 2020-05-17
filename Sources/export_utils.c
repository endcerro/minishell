/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 19:21:47 by edal--ce          #+#    #+#             */
/*   Updated: 2020/05/17 19:22:32 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_valid_export(char **params)
{
	int i;

	i = 0;
	if (params[1])
	{
		while (params[1][i])
			if (ft_isspace(params[1][i++]))
				return (0);
		if (ft_strchr(params[1], '='))
			return (1);
	}
	if (params[2] && params[2][0] == '=')
	{
		params[1] = ft_strjoinft(params[1], params[2]);
		i = 2;
		while (params[++i])
		{
			params[i - 1] = params[i];
		}
		params[i - 1] = 0;
		return (1);
	}
	return (0);
}