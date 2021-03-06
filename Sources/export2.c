/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/11 14:41:07 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/09 15:26:53 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset2(t_list *curr)
{
	int		i;

	i = -1;
	while (g_mshell.env[++i])
	{
		if (check_match(g_mshell.env[i], curr->content))
			continue ;
		free(g_mshell.env[i]);
		while (g_mshell.env[++i])
			g_mshell.env[i - 1] = g_mshell.env[i];
		g_mshell.env[i - 1] = NULL;
		break ;
	}
}

int		unset(char *param)
{
	t_list	*curr;
	t_list	*alloc;

	alloc = NULL;
	if (param)
	{
		curr = ft_lstnew(param);
		alloc = curr;
	}
	else
		curr = g_mshell.ls->next;
	while (curr && curr->type == 1)
	{
		if (ft_strchr(curr->content, '=') != NULL || *curr->content == 0)
			ft_printh(2, 1,
		"minishell: unset: \"%s\": identifiant non valable\n", curr->content);
		else
			unset2(curr);
		curr = curr->next;
	}
	if (alloc)
		free(alloc);
	return (0);
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
	if (str == NULL || *str == 0 || *str == '=')
		return (0);
	while (str && *str && *str != '=')
	{
		if (ft_isspace(*str) || ft_isascii(*str) == 0 || ft_isdigit(*str))
			return (0);
		else if (ft_isalnum(*str) == 0 && *str != '_')
			return (0);
		else if (ft_isalpha(*str))
			break ;
		++str;
	}
	while (str && *str && *str != '=')
	{
		if (ft_isspace(*str) || ft_isascii(*str) == 0)
			return (0);
		else if (ft_isalnum(*str) == 0 && *str != '_')
			return (0);
		++str;
	}
	return (1);
}
