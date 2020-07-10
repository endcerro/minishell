/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_creat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/08 15:22:37 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/10 21:40:57 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		isquote(char c)												//PROTECTED
{
	if (c == '\'' || c == '\"')
		return (c);
	return (0);
}

void	find_char(char *buff)										//PROTECTED
{
	if (buff[0] == 0)
		buff[0] = ';';
	else if (buff[0] == ';')
		buff[0] = '>';
	else if (buff[0] == '>')
		buff[0] = '<';
	else if (buff[0] == '<')
		buff[0] = '|';
	else if (buff[0] == '|')
		buff[0] = '\'';
	else if (buff[0] == '\'')
		buff[0] = '\"';
	else if (buff[0] == '\"')
		buff[0] = 0;
}

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

int		do_the_thing(t_list **new, char *buff, char **split)
{
	char	*tmpstr;
	t_list	*tmplst;

	if (0 == (tmpstr = ft_strdup(buff)))
		return (protect_pone(new, split));
	else if (0 == (tmplst = ft_lstnew(tmpstr)))
		return (protect_ptwo(new, split, tmpstr));
	ft_lstadd_back(new, tmplst);
	return (1);
}

t_list	*inner_split_loop_bis(t_list *new, t_list *curr)			//PROTECTED
{
	if (new == NULL)
		return (NULL);
	free(curr->content);
	curr->content = new->content;
	return (new);
}

t_list	*inner_split_loop(t_list *curr, char *buff, int i, int j)	//PROTECTED
{
	char	**split;
	t_list	*new;

	new = NULL;
	split = NULL;

	if (!isquote(*curr->content) && *curr->content != ' ' && curr->content[1])
		if (ft_strchr(curr->content, buff[0]))
		{
			if ((split = ft_split(curr->content, buff[0])) == NULL)
				return (NULL);
			while (curr->content[j])
			{
				while (curr->content[j] == buff[0] && ++j)
					if (do_the_thing(&new, buff, split) == 0)
						return (NULL);
				while (curr->content[j] && curr->content[j] != buff[0])
					++j;
				if (split[++i])
					if (do_the_thing(&new, split[i], split) == 0)
						return (NULL);
			}
		}
	freechar2ptr(split, 0);
	return (inner_split_loop_bis(new, curr));
}

t_list	*inner_split(t_list *lst)								//PROTECTED
{
	t_list	*curr;
	t_list	*new;
	char	buff[2];

	ft_bzero(buff, 2);
	find_char(buff);
	curr = lst;
	while (buff[0])
	{
		while (curr)
		{
			if ((new = inner_split_loop(curr, buff, -1, 0)) != 0)
			{
				ft_lstadd_back(&new, curr->next);
				curr->next = new->next;
				free(new);
			}
			curr = curr->next;
		}
		find_char(buff);
		curr = lst;
	}
	return (lst);
}

char	*get_word_lst(char *line, int *p, size_t len)			//PROTECTED
{
	int		cp;

	cp = *p;
	if (line[*p] == '\"' && ++(*p))
	{
		while (line[*p] && line[*p] != '\"' && ++len)
			++(*p);
		++(*p);
		return (ft_substr(line, cp, len + 2));
	}
	else if (line[*p] == '\'' && ++(*p))
	{
		while (line[*p] && line[*p] != '\'' && ++len)
			++(*p);
		++(*p);
		return (ft_substr(line, cp, len + 2));
	}
	else
		while (line[*p] && !ft_isspace(line[*p]) && !isquote(line[*p]) && ++len)
			++(*p);
	return (ft_substr(line, cp, len));
}

void	escape_lst(t_list *curr)								//PROTECTED
{
	while (curr && curr->type == 1)
	{
		escape_chars(curr->content, 0, 0);
		curr = curr->next;
	}
}

void	escape_chars(char *line, int bscpt, int sqnb)			//PROTECTED
{
	int i;

	i = 0;
	while (line[i])
	{
		bscpt = 0;
		while (line[i] && line[i] == '\\' && ++bscpt)
			i++;
		if (line[i] == '\"' && bscpt % 2)
			line[i] = -4;
		else if (line[i] == '\'' && bscpt % 2)
			if (sqnb == 0)
				line[i] = -3;
			else
				sqnb = 0;
		else if (line[i] == '$' && bscpt % 2)
			line[i] = -2;
		else if (line[i] == '\'')
			sqnb = (sqnb == 0) ? sqnb + 1 : sqnb - 1;
		i++;
	}
}

void	decalstr(char *str)										//PROTECTED
{
	int i;

	i = 1;
	while (str[i])
	{
		str[i - 1] = str[i];
		i++;
	}
	str[i - 1] = str[i];
}

void	de_escape_chars(char *line)								//PROTECTED
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
		i++;
	}
}

int		mergelst(t_list *curr)									//PROTECTED
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

int		correctlst(t_list *lst)									//PROTECTED
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

t_list	*split_line_lst(char *line, int i)						//PROTECTED
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
				lst->nospace = 1;
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

char	*get_lst(char *line)									//PROTECTED
{
	char	*filler;
	t_list	*out;

	if ((filler = check_finished_lst(line)) != NULL)
	{
		if ((line = ft_strjoinft(line, filler)) == NULL)
			return (NULL);
	}
	escape_chars(line, 0, 0);
	if ((out = split_line_lst(line, 0)) == NULL)
		return ((char *)(long)freeret(line, 0));
	if (tag_lst(out) == NULL)
	{
		free(line);
		ft_lstclear(&out);
		return (NULL);
	}
	g_mshell.ls = out;
	return (line);
}
