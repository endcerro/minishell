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

void	addvar(char ***vars, char *str)
{
	int		size;
	char	**newvars;

	size = 0;
	if (*vars != NULL)
		while ((*vars)[size])
			++size;
	if (!(newvars = (char **)malloc(sizeof (char *) * (size + 2))))
	{
		ft_putstr("minishell: ");
		ft_putstr(strerror(errno));
		return ;
	}
	newvars[size + 1] = NULL;
	newvars[size] = ft_strdup(str); // PAS PROTEGE
	if (*vars != NULL)
	{
		size = -1;
		while ((*vars)[++size])
		{
			newvars[size] = ft_strdup((*vars)[size]); // PAS PROTEGE
			free((*vars)[size]);
		}
		free(*vars);
	}
	*vars = newvars;
}

void	commandorvar(char ***envi, char **params, char ***vars)
{
	int x;
	int i;

	(void)envi;
	x = -1;
	while (params[++x])
	{
		i = 0;
		while (params[x][++i])
			if (params[x][i] == '=' && params[x][i - 1] != '\\')
			{
				addvar(vars, params[x]);
				break ;
			}
		if (params[x][i] == 0)
		{
			; // Execute commande
			break ;
		}
	}
}
