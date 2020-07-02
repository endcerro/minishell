/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_creat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/08 15:22:37 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/02 15:05:58 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_char(char *buff)
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
	// else if (buff[0] == '\"')
	// 	buff[0] = 0;
	// buff[1] = 0;
}

t_list	*inner_split_loop(t_list *curr, char *buff, int i, int j)
{
	char	**split;
	t_list	*new;

	new = 0;
	split = 0;
	if (curr->content[0] != '\'' && curr->content[0] != '\"' && curr->content[0] != ' ' && curr->content[1])
		if (ft_strchr(curr->content, buff[0]))
		{
			split = ft_split(curr->content, buff[0]);
			while (curr->content[j])
			{
				while (curr->content[j] == buff[0] && ++j)
					ft_lstadd_back(&new, ft_lstnew(ft_strdup(buff)));
				while (curr->content[j] && curr->content[j] != buff[0])
					j++;
				if (split[++i])
				{
					ft_lstadd_back(&new, ft_lstnew(split[i]));
				}
			}
		}
	free(split);
	if (new == 0)
		return (0);
	free(curr->content);
	curr->content = new->content;
	return (new);
}


t_list	*inner_split(t_list *lst)
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

char	*get_word_lst(char *line, int *p)
{
	size_t	len;
	int		cp;

	cp = *p;
	len = 0;
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
	{
		while (line[*p] && !ft_isspace(line[*p]) && line[*p] != '\'' && line[*p] != '\"' && ++len)
			++(*p);
	}
	return (ft_substr(line, cp, len));
}

void    escape_lst (t_list *curr)
{
	while (curr && curr->type == 1)
	{
		escape_chars(curr->content);
		curr = curr->next;
	}
}

void 	escape_chars(char *line)
{
	int i;
	int bscpt;
	int sqnb;

	bscpt = 0;
	i = 0;
	sqnb = 0;
	while(line[i])
	{
		bscpt = 0;
		while(line[i] && line[i] == '\\')
		{
			bscpt++;
			i++;
		}
		if (line[i] == '\"' && bscpt % 2)
		{
			line[i] = -4;
		}
		else if(line[i] == '\'' && bscpt % 2)
		{
			if(sqnb == 0)
				line[i] = -3;
			else
				sqnb = 0;
		}
		else if (line[i] == '$' && bscpt % 2)
		{
			line[i] = -2;
		}
		else if(line[i] == '\'')
		{
			if (sqnb == 0)
				sqnb++;
			else
				sqnb--;
		}
		i++;
	}
}

void	decalstr(char *str)
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

void 	de_escape_chars(char *line)
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

int		isquote(char c)
{
	if(c == '\'')
		return '\'';
	if(c == '\"')
		return '\"';
	return 0;
}

char	*list_to_str(t_list *lst, int depth)
{
	char *out;
	int i;
	out = ft_strdup("");
	i = 0;
	while (lst && i < depth)
	{
		out = ft_strjoinf1(out, lst->content);
		lst = lst->next;
		i++;
	}
	return out;
}

void mergelst(t_list *curr)
{
	t_list *tmp;

	while (curr)
	{
		if (curr->nospace == 1 && curr->next && curr->type == 1 && curr->next->type == 1)
		{
			curr->content = ft_strjoinf1(curr->content, curr->next->content);
			curr->nospace = curr->next->nospace;
			tmp = curr->next->next;
			ft_lstdelone(curr->next);
			curr->next = tmp;
			continue;
		}
		curr = curr->next;
	}
}

void	correctlst(t_list *lst)
{
	t_list *prev;
	t_list *tmp;
	prev = 0;
	while(lst && lst->content)
	{
		if(lst->content[0] == '\"' || lst->content[0] == '\'')
			trim_quotes(lst->content);
		parse_bs(lst->content);
		de_escape_chars(lst->content);
		if(lst->content[0] == 0 && prev)
		{
			if(lst->next)
			{
				prev->content = ft_strjoinf1(prev->content, lst->next->content);
				prev->next = lst->next->next;
				ft_lstdelone(lst->next);
				ft_lstdelone(lst);
			}
			else
			{
				ft_lstdelone(lst);
				prev->next = 0;
			}
			lst = prev;
		}
		prev = lst;
		lst = lst->next;
	}
}

t_list	*split_line_lst(char *line)					//MODIF
{
	t_list	*f_lst;
	t_list	*lst;
	int		i;

	i = 0;
	f_lst = NULL;
	escape_chars(line);
	while ((size_t)i < ft_strlen(line))
	{
		if (ft_isspace(line[i]))
			++i;
		else
		{
			if ((lst = ft_lstnew_p(get_word_lst(line, &i))) == NULL
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

char	*get_lst(char *line)		//PROTECTED
{
	char	*filler;
	t_list	*out;
	if ((filler = check_finished_lst(line)) != NULL)
	{
		if ((line = ft_strjoinft(line, filler)) == NULL)
			return (NULL);
	}
	if ((out = split_line_lst(line)) == NULL)
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
