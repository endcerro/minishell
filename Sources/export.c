/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 19:18:39 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/26 17:22:17 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		exportstuff(t_list *curr, char **tmp, char ***n_envi, char *param)
{
	int i;

	i = -1;
	while (g_mshell.env[++i])
		if (!check_match(g_mshell.env[i], curr->content))
		{
			*tmp = g_mshell.env[i];
			g_mshell.env[i] = ft_strdup(curr->content);
			if (g_mshell.env[i] == 0)
			{
				g_mshell.env[i] = *tmp;
				return (1);
			}
			if (param != NULL)
				free(curr);
			return (freeret(*tmp, 0));
		}
	if (!(*n_envi = malloc(sizeof(char *) * (i + 2))))
		return (1);
	return (-1);
}

int		exportstuffbis(char ***n_envi, t_list **curr)
{
	int i;

	i = -1;
	while (g_mshell.env[++i])
		(*n_envi)[i] = g_mshell.env[i];
	(*n_envi)[i] = ft_strdup((*curr)->content);
	if ((*n_envi)[i] == 0)
		return (freechar2ptr(*n_envi, 1));
	(*n_envi)[++i] = 0;
	free(g_mshell.env);
	g_mshell.env = *n_envi;
	*curr = (*curr)->next;
	return (0);
}

int		exportstuffter(t_list *curr, char *tmp)
{
	curr->content = ft_strjoinf1(curr->content, "=");
	if (curr->content == 0)
		return (1);
	curr->content = ft_strjoinf1(curr->content, tmp);
	if (curr->content == 0)
		return (1);
	return (0);
}

int		exportstuffquater(t_list *curr, char *tmp, char **n_envi, char *param)
{
	int i;

	while (curr && curr->type == 1)
	{
		if (ft_strchr(curr->content, '=') == NULL)
		{
			if ((tmp = vars(curr->content)) != NULL)
			{
				if (exportstuffter(curr, tmp) == 1)
					return (1);
			}
			else if (env(curr->content))
			{
				curr = curr->next;
				continue ;
			}
		}
		unset_var(curr->content);
		if ((i = exportstuff(curr, &tmp, &n_envi, param)) != -1)
			return (i);
		if (exportstuffbis(&n_envi, &curr) == 1)
			return (1);
	}
	return (0);
}

int		export(char *param)
{
	char	**n_envi;
	char	*tmp;
	t_list	*curr;

	n_envi = NULL;
	tmp = NULL;
	if (param != NULL)
		curr = ft_lstnew(param);
	else
		curr = g_mshell.ls->next;
	if (curr && check_valid_export(curr->content) == 0)
	{
		if (param != NULL)
			free(curr);
		else
			param = curr->content;
		return (ft_printh(2, 1,
				"minishell: export: \'%s\': not a valid identifier\n", param));
	}
	if (curr == 0)
		return (exportlst(g_mshell.env));
	return (exportstuffquater(curr, tmp, n_envi, param));
}
