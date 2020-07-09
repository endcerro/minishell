/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 19:18:39 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/09 19:14:55 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		unset(void)								//PROTECTED
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

void	print_export(char **env2)				//PROTECTED
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

int		exportlst(char **envi)					//PROTECTED
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

int		export(char *param)				//PROTECTED
{
	char	**n_envi;
	char	*tmp;
	t_list	*curr;

	n_envi = NULL;
	tmp = NULL;
	curr = g_mshell.ls->next;
	if (param != NULL)
		curr = ft_lstnew(param);
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
