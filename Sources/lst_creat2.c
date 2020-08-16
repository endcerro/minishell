/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_creat2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/11 14:46:28 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/16 17:51:30 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			protect_pone(t_list **new, char **split, char *str, int mode)
{
	if (mode == 1)
		free(str);
	ft_lstclear(new);
	freechar2ptr(split, 0);
	return (0);
}

int			do_the_thing(t_list **new, char *buff, char **split, t_list **tmlst)
{
	char	*tmpstr;

	if (0 == (tmpstr = ft_strdup(buff)))
		return (protect_pone(new, split, tmpstr, 0));
	else if (0 == (*tmlst = ft_lstnew(tmpstr)))
		return (protect_pone(new, split, tmpstr, 1));
	(*tmlst)->nospace = 1;
	ft_lstadd_back(new, *tmlst);
	return (1);
}

t_list		*inner_split_loop_bis(t_list *new, t_list *curr)
{
	if (new == NULL)
		return (NULL);
	free(curr->content);
	curr->content = new->content;
	return (new);
}

t_innsplitl	initinnsplitl(t_list *curr)
{
	t_innsplitl	s;

	s.status = curr->nospace;
	s.new = NULL;
	s.tml = NULL;
	s.split = NULL;
	return (s);
}

t_list		*inner_split_loop(t_list *curr, char *buff, int i, int j)
{
	t_innsplitl	s;

	s = initinnsplitl(curr);
	if (!isquote(*curr->content) && *curr->content != ' ' && curr->content[1])
		if (ft_strchr(curr->content, buff[0]))
		{
			if ((s.split = ft_split(curr->content, buff[0])) == NULL)
				return (NULL);
			curr->nospace = 1;
			while (curr->content[j])
			{
				while (curr->content[j] == buff[0] && ++j)
					if (do_the_thing(&s.new, buff, s.split, &s.tml) == 0)
						return (NULL);
				while (curr->content[j] && curr->content[j] != buff[0])
					++j;
				if (s.split[++i])
					if (do_the_thing(&s.new, s.split[i], s.split, &s.tml) == 0)
						return (NULL);
			}
			if (s.tml)
				s.tml->nospace = s.status;
		}
	freechar2ptr(s.split, 0);
	return (inner_split_loop_bis(s.new, curr));
}
