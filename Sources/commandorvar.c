/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandorvar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpottier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 15:29:01 by hpottier          #+#    #+#             */
/*   Updated: 2019/11/12 17:03:45 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	addvar(char ***vars)
{
	(void)vars;
}

void	commandorvar(char ***envi, char **params, char ***vars)
{
	int x;
	int i;

	(void)envi;
	x = -1;
	while (params[++x])
	{
		i = -1;
		while (params[x][++i])
			if (params[x][i] == '=')
			{
				addvar(vars); // Gerer variable
				break ;
			}
		if (params[x][i] == 0)
		{
			; // Execute commande
			break ;
		}
	}
	return (vars);
}
