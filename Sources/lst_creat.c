/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_creat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/08 15:22:37 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/01 13:26:36 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	de_escape_chars(char *line)
{
	int i;

	i = 0;
	while (line[i] != 0)
	{
		if (line[i] == -4)
		{
			line[i] = '\"';
		}
		else if (line[i] == -2)
		{
			line[i] = '$';
		}
		else if (line[i] == -3)
		{
			line[i] = '\'';
		}
		else if (line[i] == -5)
		{
			line[i] = ';';
		}
		else if (line[i] == -6)
		{
			line[i] = '>';
		}
		else if (line[i] == -7)
		{
			line[i] = '<';
		}
		i++;
	}
}

int		mergelst(t_list *curr)
{
	t_list *tmp;

	while (curr)
	{
		if (curr->nospace == 1 && curr->next && curr->type == 1
			&& curr->next->type == 1)
		{
			curr->content = ft_strjoinf1(curr->content, curr->next->content);
			if (curr->content == 0)
				return (1);
			curr->nospace = curr->next->nospace;
			tmp = curr->next->next;
			ft_lstdelone(curr->next);
			curr->next = tmp;
			continue;
		}
		curr = curr->next;
	}
	return (0);
}

int		correctlst(t_list *lst)
{
	while (lst && lst->content)
	{
		trim_quotes(lst->content);
		parse_bs(lst->content);
		de_escape_chars(lst->content);
		lst = lst->next;
	}
	return (0);
}

t_list	*split_line_lst(char *line, int i)
{
	t_list	*f_lst;
	t_list	*lst;

	f_lst = NULL;
	while ((size_t)i < ft_strlen(line))
	{
		if (ft_isspace(line[i]))
			++i;
		else
		{
			if ((lst = ft_lstnew_p(get_word_lst(line, &i, 0))) == NULL
				|| lst->content == NULL)
				return ((t_list *)(long)ft_lstclear(&f_lst));
			else if (line[i] != ' ' && line[i] != 0)
			{
				lst->nospace = 1;
			}
			if (f_lst == NULL && (f_lst = lst))
				lst = NULL;
			else
				ft_lstadd_back(&f_lst, lst);
		}
	}
	if (inner_split(f_lst) == NULL)
		return ((t_list *)(long)ft_lstclear(&f_lst));

	return (f_lst);
}

void joindrdir(t_list *in)
{
	while(in)
	{
		if (in->content[0] == '>'&& in->nospace && in->next && in->next->content[0] == '>')
		{
			in->content = ft_strdup(">>");
			t_list *tmp = in->next->next;
			ft_lstdelone(in->next);
			in->next = tmp;
		}
		in = in->next;
	}
}

char	*get_lst(char *line)
{
	char	*filler;
	t_list	*out;
	int		err;

	err = 0;
	if ((filler = check_finished_lst(line, &err)) != NULL)
		if ((line = ft_strjoinft(line, filler)) == NULL)
			return (NULL);
	if (err == -1)
	{
		free(line);
		return (NULL);
	}
	escape_chars(line, 0, 0);
	if ((out = split_line_lst(line, 0)) == NULL)
		return ((char *)(long)freeret(line, 0));
	joindrdir(out);
	if (tag_lst(out) == NULL)
	{
		free(line);
		ft_lstclear(&out);
		return (NULL);
	}
	g_mshell.ls = out;
	return (line);
}
