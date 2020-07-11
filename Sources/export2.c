/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/11 14:41:07 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/11 14:54:24 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		unset(void)
{
	int		i;
	t_list	*curr;

	curr = g_mshell.ls->next;
	while (curr && curr->type == 1)
	{
		i = 0;
		while (g_mshell.env[i])
		{
			if (!check_match(g_mshell.env[i], curr->content))
			{
				free(g_mshell.env[i]);
				while (g_mshell.env[++i])
					g_mshell.env[i - 1] = g_mshell.env[i];
				g_mshell.env[i - 1] = 0;
				break ;
			}
			i++;
		}
		curr = curr->next;
	}
	return (0);
}

void	print_export(char **env2)
{
	int x;
	int y;

	x = -1;
	while (env2[++x])
	{
		y = 0;
		ft_putstr("declare -x ");
		while (env2[x][y] && env2[x][y] != '=')
			++y;
		write(1, env2[x], y);
		if (env2[x][y] == '=' && env2[x][y + 1] == 0)
			write(1, "=\"\"", 3);
		else if (env2[x][y] != 0 && env2[x][y + 1] != 0)
		{
			write(1, "=\"", 2);
			ft_putstr(env2[x] + y + 1);
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
	}
}

int		exportlst(char **envi)
{
	int		y;
	char	**env2;

	y = 0;
	while (envi[y])
		++y;
	if ((env2 = (char **)malloc(sizeof(char *) * (y + 1))) == NULL)
		return (ft_printh(2, 1, "%s\n", strerror(errno)));
	y = -1;
	while (envi[++y])
	{
		if ((env2[y] = ft_strdup(envi[y])) == 0)
		{
			freechar2ptr(env2, 0);
			return (ft_printh(2, 1, "%s\n", strerror(errno)));
		}
	}
	env2[y] = NULL;
	quicks(env2, 0, y - 1);
	print_export(env2);
	freechar2ptr(env2, 0);
	return (0);
}

int		unset_var(char *target)
{
	int		i;

	i = 0;
	if (g_mshell.vars != NULL)
		while (g_mshell.vars[i])
		{
			if (!check_match(g_mshell.vars[i], target))
			{
				free(g_mshell.vars[i]);
				while (g_mshell.vars[++i])
					g_mshell.vars[i - 1] = g_mshell.vars[i];
				g_mshell.vars[i - 1] = 0;
				return (0);
			}
			i++;
		}
	return (0);
}

int		check_valid_export(char *str)
{
	while (str && *str && *str != '=')
	{
		if (ft_isspace(*str) == 1)
			return (0);
		++str;
	}
	return (1);
}