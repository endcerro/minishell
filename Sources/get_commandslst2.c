/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commandslst2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/11 14:43:05 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/04 21:11:49 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int expand_t(char **str)
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

int		expand_vars(t_list *curr)
{
	t_list *prev;

	prev = 0;
	while (curr && curr->type != 3 && curr->type != 6)
	{
		if (curr->content && curr->content[0] != '\'')
		{
			if (expand_t(&(curr->content)))
				return (1);
			parse_env_ls(&(curr->content), 0, curr);
			if (prev && (prev->type == 2 || prev->type == 4 || prev->type == 5))
			{
				if (checkspace(curr->content))
					return (ft_printh(2, 1, "minishell: ambiguous redirect\n"));
			}
			if (curr->content && curr->content[0] == 0)
			{
				if (prev && (prev->type == 2 || prev->type == 4 || prev->type == 5))
				{
					return (ft_printh(2, 1, "minishell: ambiguous redirect\n"));
				}
				else if (prev)
				{
					prev->next = curr->next;
					ft_lstdelone(curr);
				}
				curr = prev;
			}
		}
		if (curr == 0 || curr->content == 0)
			return (1);
		prev = curr;
		curr = curr->next;
	}
	return (0);
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

void	trimbs_loop(t_list *curr, int i)
{
	int j;
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
			if (curr->content[0] == '\"' && curr->content[j] < 0 && curr->content[j] != -3)
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
	while (curr && curr->content) //&& curr->type == 1)
	{
		if (curr->content[0] != '\'' && !curr->rawtext)
			trimbs_loop(curr, 0);
		curr = curr->next;
	}
}

int		check_valid(t_list *lst)
{
	int 	cp_r;
	int 	cp_d;
	t_list 	*first;


	first = lst;
	cp_r = 0;
	cp_d = 0;
	while (lst)
	{
		if (lst->type == 3 || lst->type == 6)
		{
			cp_d++;
			if ((lst->type == 6 && lst->next == NULL) || lst == first)
			{
				g_mshell.exitcode = 258;
				return (ft_printh(2, 0, "minishell: syntax error near unexpected token `|'\n"));
			}
		}
		else if (lst->type != 1)
		{
			cp_r++;
			if (lst->next == 0 || lst->next->content == 0 || lst->next->content[0] == 0)
			{
				g_mshell.exitcode = 258;
				return (ft_printh(2, 0, "minishell: syntax error near unexpected token `newline'\n"));
			}
		}
		else
		{
			cp_r = 0;
			cp_d = 0;
		}
		if ((cp_d > 1 || cp_r > 1) || (cp_r + cp_d > 1))
		{
			g_mshell.exitcode = 258;
			return (ft_printh(2, 0, SYNTERR, lst->content));
		}
		lst = lst->next;
	}
	return (1);
}
