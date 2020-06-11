/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_creat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/08 15:22:37 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/11 19:45:48 by edal--ce         ###   ########.fr       */
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
		buff[0] = 0;
}

t_list	*inner_split_loop(t_list *curr, char *buff, int i, int j)
{
	char	**split;
	t_list	*new;

	new = 0;
	split = 0;
	if (ft_isalnum(curr->content[0]) && curr->content[1])
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
					ft_lstadd_back(&new, ft_lstnew(split[i]));
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
	if (line[*p] == '"' && ++(*p))
	{
		while (line[*p] && line[*p] != '"' && ++len)
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
		while (line[*p] && !ft_isspace(line[*p]) && ++len)
			++(*p);
	return (ft_substr(line, cp, len));
}

void 	testfun(char *line)
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
void 	testfun2(char *line)
{
	int i;

	i = 0;
	while(line[i])
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

void correctlst(t_list *lst)
{
	while(lst && lst->content)
	{
		testfun2(lst->content);
		lst = lst->next;
	}
}

t_list	*split_line_lst(char *line)					//ICI
{
	t_list	*f_lst;
	t_list	*lst;
	int		i;

	i = 0;
	f_lst = NULL;
	testfun(line);
	while ((size_t)i < ft_strlen(line))
	{
		if (ft_isspace(line[i]))
			++i;
		else
		{
			if ((lst = ft_lstnew_p(get_word_lst(line, &i))) == NULL
				|| lst->content == NULL)
				return ((t_list *)(long)ft_lstclear(&f_lst));
			if (f_lst == NULL && (f_lst = lst))
				lst = NULL;
			else
				ft_lstadd_back(&f_lst, lst);
		}
	}
	if (inner_split(f_lst) == NULL)
		return ((t_list *)(long)ft_lstclear(&f_lst));
	// correctlst(f_lst);
	// testfun2(line);

	return (f_lst);
}

char	*get_lst(char *line)		//PROTECTED
{
	char	*filler;
	t_list	*out;
	// printf("Line = %s\n",line );
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
	// printf("Line = %s\n",line );
	g_mshell.ls = out;
	return (line);
}
