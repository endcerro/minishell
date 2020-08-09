/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functionsnstuff.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/08 19:04:42 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/09 16:47:54 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*rethomedir(void)
{
	int	x;

	x = 0;
	while (g_mshell.env[x] && ft_strncmp("HOME=", g_mshell.env[x], 5) != 0)
		++x;
	if (g_mshell.env[x])
		return (g_mshell.env[x] + 5);
	return (NULL);
}

int		retunset(char *str)
{
	unset(str);
	return (ft_printh(2, 1, "minishell: cd: %s\n", strerror(errno)));
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

void	deconechar(char *str)
{
	int i;

	i = 0;
	if (*str == 0)
		return ;
	while (str[++i])
		str[i - 1] = str[i];
	str[i - 1] = str[i];
}

int		checkspace(char *str)
{
	while (*str)
	{
		if (ft_isspace(*str))
			return (1);
		++str;
	}
	return (0);
}
