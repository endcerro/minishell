/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_creat2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/11 14:46:28 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/16 17:25:03 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		protect_pone(t_list **new, char **split)
{
	ft_lstclear(new);
	freechar2ptr(split, 0);
	return (0);
}

int		protect_ptwo(t_list **new, char **split, char *str)
{
	free(str);
	return (protect_pone(new, split));
}

int		do_the_thing(t_list **new, char *buff, char **split, t_list **tmplst)
{
	char	*tmpstr;

	if (0 == (tmpstr = ft_strdup(buff)))
		return (protect_pone(new, split));
	else if (0 == (*tmplst = ft_lstnew(tmpstr)))
		return (protect_ptwo(new, split, tmpstr));
	(*tmplst)->nospace = 1;
	ft_lstadd_back(new, *tmplst);
	return (1);
}

t_list	*inner_split_loop_bis(t_list *new, t_list *curr)
{
	if (new == NULL)
		return (NULL);
	free(curr->content);
	curr->content = new->content;
	return (new);
}

t_list	*inner_split_loop(t_list *curr, char *buff, int i, int j)
{
	char	**split;
	t_list	*new;
	int		status;
	t_list	*tmplst;

	status = curr->nospace;
	new = NULL;
	tmplst = NULL;
	split = NULL;
	if (!isquote(*curr->content) && *curr->content != ' ' && curr->content[1])
		if (ft_strchr(curr->content, buff[0]))
		{
			if ((split = ft_split(curr->content, buff[0])) == NULL)
				return (NULL);
			curr->nospace = 1;
			while (curr->content[j])
			{
				while (curr->content[j] == buff[0] && ++j)
					if (do_the_thing(&new, buff, split, &tmplst) == 0)
						return (NULL);
				while (curr->content[j] && curr->content[j] != buff[0])
					++j;
				if (split[++i])
					if (do_the_thing(&new, split[i], split, &tmplst) == 0)
						return (NULL);
			}
			if (tmplst)
				tmplst->nospace = status;
		}
	freechar2ptr(split, 0);
	return (inner_split_loop_bis(new, curr));
}
