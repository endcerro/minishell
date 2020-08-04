/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commandslst2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/11 14:43:05 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/04 17:45:01 by hpottier         ###   ########.fr       */
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
			if (curr->content && curr->content[0] == 0)
			{
				if (prev)
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
			if (curr->content[0] == '\"' && curr->content[j] < 0)
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
		if (curr->content[0] != '\'')
			trimbs_loop(curr, 0);
		curr = curr->next;
	}
}

int		check_valid(t_list *lst)
{
	int cp_r;
	int cp_d;

	cp_r = 0;
	cp_d = 0;
	while (lst)
	{
		ft_lstprint(lst);
		if (lst->type == 3 || lst->type == 6)
			cp_d++;
		else if (lst->type != 1)
			cp_r++;
		else
		{
			cp_r = 0;
			cp_d = 0;
		}
		if ((cp_d > 1 || cp_r > 1))
			return (ft_printh(2, 0, SYNTERR, lst->content));
		lst = lst->next;
	}
	return (1);
}
