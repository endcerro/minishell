/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 19:08:17 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/05 19:09:27 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		checkexport(char *var)
{
	int x;

	x = 0;
	while (var[x] && var[x] != '=')
		++x;
	if (var[x] == 0)
		return (0);
	return (1);
}

char 	*env(char *request)
{
	int x;

	x = 0;
	// if (g_mshell.params[1] && request == NULL)
	// 	ft_putstr("minishell: env: too many arguments\n");
	// else
//	{
		while (g_mshell.env[x])
		{
			if(request != NULL)
			{
				if (!ft_strncmp(g_mshell.env[x], request, wordlen(request)))
					return (g_mshell.env[x] + wordlen(request));
			}
			else if (checkexport(g_mshell.env[x]) == 1)
			{
				// printf("here\n");
				ft_putsendl(g_mshell.env[x]);
			}
			++x;
		}
//	}
	return (NULL);
}