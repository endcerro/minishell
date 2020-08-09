/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commandslst2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/11 14:43:05 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/09 20:09:31 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		expand_t(char **str)
{
	char *home;

	if (*str && (*str)[0] == '~' && (*str)[1] == 0)
	{
		free(*str);
		*str = NULL;
		if ((home = rethomedir()) == NULL)
			return (ft_printh(2, 1, "minishell: $HOME not set\n"));
		*str = ft_strdup(home);
		if (*str == 0)
			return (ft_printh(2, 1, "minishell: %s\n", strerror(errno)));
	}
	return (0);
}

int		expand_vars2(t_list **curr, t_list *pr)
{
	if (expand_t(&((*curr)->content)))
		return (1);
	parse_env_ls(&((*curr)->content), 0, (*curr));
	if (pr && (pr->type == 2 || pr->type == 4 || pr->type == 5))
		if (checkspace((*curr)->content))
			return (ft_printh(2, 1, "minishell: ambiguous redirect\n"));
	if ((*curr)->content && (*curr)->content[0] == 0)
	{
		if (pr && (pr->type == 2 || pr->type == 4 || pr->type == 5))
		{
			return (ft_printh(2, 1, "minishell: ambiguous redirect\n"));
		}
		else if (pr)
		{
			pr->next = (*curr)->next;
			ft_lstdelone((*curr));
		}
		(*curr) = pr;
	}
	return (0);
}

int		expand_vars(t_list *curr)
{
	t_list *pr;

	pr = 0;
	while (curr && curr->type != 3 && curr->type != 6)
	{
		if (curr->content && curr->content[0] != '\'')
			if (expand_vars2(&curr, pr))
				return (1);
		if (curr == 0 || curr->content == 0)
			return (1);
		pr = curr;
		curr = curr->next;
	}
	return (0);
}

void	trimbs_loop(t_list *curr, int i, int j)
{
	int qtcp;

	while (curr->content[i])
	{
		if (curr->content[i] == '\\')
		{
			qtcp = 0;
			j = i - 1;
			while (curr->content[++j] == '\\')
				qtcp++;
			if ((qtcp != 1 && qtcp % 2 == 0) || curr->content[0] == '\"')
				qtcp /= 2;
			else if (qtcp != 1)
				qtcp = (qtcp - 1) / 2 + 1;
			if (curr->content[0] == '\"' && curr->content[j] < 0
				&& curr->content[j] != -3)
				qtcp++;
			j = -1;
			while (++j < qtcp)
				deconechar(curr->content + i);
			i += qtcp != 0 ? qtcp : 1;
		}
		else
			i++;
	}
}

void	trimbs(t_list *curr)
{
	while (curr && curr->content)
	{
		if (curr->content[0] != '\'' && !curr->rawtext)
			trimbs_loop(curr, 0, 0);
		curr = curr->next;
	}
}

int		check_valid(t_list *lst, int cp_r, int cp_d, t_list *first)
{
	while (lst)
	{
		if ((lst->type == 3 || lst->type == 6) && ++cp_d)
		{
			if ((lst->type == 6 && lst->next == NULL) || lst == first)
				return ((g_mshell.exitcode = ft_printh(2, 2,
													SYNTERR2, lst->content)));
		}
		else if (lst->type != 1 && ++cp_r)
		{
			if (lst->next == 0 || lst->next->content == 0
				|| lst->next->content[0] == 0)
				return ((g_mshell.exitcode = ft_printh(2, 2, SYNTERR3)));
		}
		else if ((cp_r = 0) || 1)
			cp_d = 0;
		if ((cp_d > 1 || cp_r > 1) || (cp_r + cp_d > 1 && lst->type != 2
									&& lst->type != 4 && lst->type != 5))
			return ((g_mshell.exitcode = ft_printh(2, 2, SYNTE, lst->content)));
		lst = lst->next;
	}
	return (0);
}
