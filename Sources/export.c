/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 19:18:39 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/24 19:21:29 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		unset(void)
{
	int		i;
	t_list	*curr;

	curr = g_mshell.ls->next;
	
	// if (curr == 0 || curr->next != 0)
	// {
	// 	ft_putstr("minishell: unset: wrong number of arguments\n");
	// 	return (1);
	// }
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

void	exportlst(char **envi)					//MALLOC PROTECTED
{
	int		y;
	char	**env2;

	y = 0;
	while (envi[y])
		++y;
	if ((env2 = (char **)malloc(sizeof(char *) * (y + 1))) == NULL)
	{
		ft_putstr(strerror(errno));
		return ;
	}
	y = -1;
	while (envi[++y])
	{
		if ((env2[y] = ft_strdup(envi[y])) == 0)
		{
			freechar2ptr(env2);
			return ;
		}
	}
	env2[y] = NULL;
	quicks(env2, 0, y - 1);
	print_export(env2);
	freechar2ptr(env2);
}

int		unset_var(char *target)			//PROTECTED
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

int		export(char *param)			//PROTECTED
{
	int		i;
	char	**n_envi;
	char	*tmp;
	t_list	*curr;

	curr = g_mshell.ls->next;
	if (param != NULL)
	{
		curr = ft_lstnew(param);
	}
	i = 0;
	if (curr == 0)
	{
		exportlst(g_mshell.env); // PROTECTED
		return (0);
	}
	while (curr && curr->type == 1)
	{
		if (ft_strchr(curr->content, '=') == NULL)
		{
			tmp = vars(curr->content);
			if (tmp)
			{
				curr->content = ft_strjoinf1(curr->content, "=");
				if (curr->content == 0)
					return (1);
				curr->content = ft_strjoinf1(curr->content, tmp);
				if (curr->content == 0)
					return (1);
			}
			else if (env(curr->content))
			{
				curr = curr->next;
				continue ;
			}
		}
		unset_var(curr->content);
		while (g_mshell.env[i])
		{
			if (!check_match(g_mshell.env[i], curr->content))
			{
				tmp = g_mshell.env[i];
				g_mshell.env[i] = ft_strdup(curr->content); // PAS PROTEGE
				if (g_mshell.env[i] == 0)
				{
					g_mshell.env[i] = tmp;
					return (1);
				}
				if (param != NULL)
					free(curr);
				return (freeret(tmp, 0));
			}
			i++;
		}
		if (!(n_envi = malloc(sizeof(char *) * (i + 2))))
			return (1);
		i = -1;
		while (g_mshell.env[++i])
			n_envi[i] = g_mshell.env[i];
		n_envi[i] = ft_strdup(curr->content); // PAS PROTEGE
		if (n_envi[i] == 0)
		{
			freechar2ptr(n_envi);
			return (1);
		}
		n_envi[++i] = 0;
		free(g_mshell.env);
		g_mshell.env = n_envi;
		curr = curr->next;
	}
	return (0);
}
