/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_creat3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/11 14:47:05 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/16 17:37:59 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*get_word_lst(char *line, int *p, size_t len)
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

void	escape_chars2(char **line, int i, int bscpt)
{
	if ((*line)[i] == '>' && bscpt % 2)
		(*line)[i] = -6;
	else if ((*line)[i] == '|' && bscpt % 2)
		(*line)[i] = -8;
	else if ((*line)[i] == '<' && bscpt % 2)
		(*line)[i] = -7;
	else if ((*line)[i] == ' ' && bscpt % 2)
		(*line)[i] = -9;
	else if ((*line)[i] == '\t' && bscpt % 2)
		(*line)[i] = -10;
	else if ((*line)[i] == '\"' && bscpt % 2)
		(*line)[i] = -4;
	else if ((*line)[i] == '\r' && bscpt % 2)
		(*line)[i] = -11;
	else if ((*line)[i] == '\v' && bscpt % 2)
		(*line)[i] = -12;
	else if ((*line)[i] == '$' && bscpt % 2 && line[i + 1])
		(*line)[i] = -2;
}

void	escape_chars(char *line, int i, int sqnb, char *cpt)
{
	int bscpt;
	int len;

	len = ft_strlen(line);
	while (line && i < len && line[++i] && !(bscpt = 0))
	{
		while (line[i] && line[i] == '\\' && ++bscpt)
			i++;
		if (line[i] == 0 && bscpt % 2 && cpt && ++(cpt[2]))
			return ;
		if (line[i] == ';' && bscpt % 2)
			line[i] = -5;
		else if (line[i] == '\f' && bscpt % 2)
			line[i] = -13;
		else if (line[i] == '\'' && bscpt % 2)
		{
			if (sqnb == 0)
				line[i] = -3;
			else
				sqnb = 0;
		}
		else if (line[i] == '\'')
			sqnb = (sqnb == 0) ? sqnb + 1 : sqnb - 1;
		escape_chars2(&line, i, bscpt);
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
