/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:45 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/05 16:08:28 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_list	*inner_split(t_list *lst)
{
	t_list	*curr;
	t_list	*new;
	char	**split;
	int		i;
	int		j;
	char	buff[2];
	t_list	*tmp;

	ft_bzero(buff, 2);
	find_char(buff);
	curr = lst;
	while (buff[0])
	{
		while (curr)
		{
			new = 0;
			if (curr->content[0] != '\'' && curr->content[0] != '\"' && curr->content[1])
			{
				if (ft_strchr(curr->content, buff[0]))
				{
					if ((split = ft_split(curr->content, buff[0])) == NULL)
						return (NULL);
					i = -1;
					j = 0;
					while (curr->content[j])
					{
						while (curr->content[j] == buff[0] && ++j)
						{
							if ((tmp = ft_lstnew(ft_strdup(buff))) == NULL)
								return (NULL);
							else if (tmp->content == NULL)
							{
								ft_lstdelone(tmp);
								return (NULL);
							}
							ft_lstadd_back(&new, tmp);
						}
						while (curr->content[j] && curr->content[j] != buff[0])
							j++;
						if (split[++i])
						{
							if ((tmp = ft_lstnew(split[i])) == NULL)
								return (NULL);
							ft_lstadd_back(&new, ft_lstnew(split[i]));
						}
					}
					free(split);
				}
			}
			if (new)
			{
				free(curr->content);
				curr->content = new->content;
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

t_list	*split_line_lst(char *line)
{
	t_list	*f_lst;
	t_list	*lst;
	int		i;

	i = 0;
	f_lst = NULL;
	while ((size_t)i < ft_strlen(line))
	{
		if (ft_isspace(line[i]))
			++i;
		else
		{
			if ((lst = ft_lstnew(get_word_lst(line, &i))) == NULL)
			{
				ft_lstclear(&f_lst);
				return (NULL);
			}
			if (f_lst == NULL)
			{
				f_lst = lst;
				lst = NULL;
			}
			else
				ft_lstadd_back(&f_lst, lst);
		}
	}
	if (inner_split(f_lst) == NULL)
	{
		ft_lstclear(&f_lst);
		return (NULL);
	}
	return (f_lst);
}

t_list	*tag_lst(t_list *lst)
{
	t_list	*cr;
	t_list	*cpy;

	cr = lst;
	while (cr)
	{
		if (ft_strcmp(cr->content, ">") == 0 && (cr->type = 2))
		{
			if (cr->next && ft_strcmp(cr->next->content, ">") == 0)
			{
				cr->type = 4;
				cr->content = ft_strjoinft(cr->content, cr->next->content);
				if (cr->content == NULL)
					return (NULL);
				cpy = cr->next->next;
				free(cr->next);
				cr->next = cpy;
			}
		}
		else if (ft_strcmp(cr->content, ";") == 0)
			cr->type = 3;
		else if (ft_strcmp(cr->content, "<") == 0)
			cr->type = 5;
		else if (ft_strcmp(cr->content, "|") == 0)
			cr->type = 6;
		cr = cr->next;
	}
	return (lst);
}

char	*get_lst(char *line)
{
	char	*filler;
	t_list	*out;

	if ((filler = check_finished_lst(line)) != NULL) // PAS PROTEGE
	{
		if ((line = ft_strjoinft(line, filler)) == NULL)
			return (NULL);
	}
	if ((out = split_line_lst(line)) == NULL)
		return (NULL);
	if (tag_lst(out) == NULL)
	{
		ft_lstclear(&out);
		return (NULL);
	}
	g_mshell.ls = out;
	return (line);
}

int		echo_ls(void)
{
	int		ret;
	t_list	*curr;

	ret = 0;
	curr = g_mshell.ls->next;
	if (curr && ft_strcmp(curr->content, "-n") == 0)
		curr = curr->next;
	else
		ret = 1;
	while (curr && curr->type == 1)
	{
		ft_putstr(curr->content);
		if (curr->next)
			write(1, " ", 1);
		curr = curr->next;
	}
	if (ret)
		write(1, "\n", 1);
	return (0);
}

void	expand_vars(t_list *lst)
{
	t_list *curr;

	curr = lst;
	while (curr && curr->type != 3)
	{
		parse_env_ls(&(curr->content));
		curr = curr->next;
	}
}

void	checkinput_ls(void)
{
	t_list	*curr;
	t_list	*copy;

	curr = g_mshell.ls;
	if (curr == 0)
		return ;
	check_rdir();
	expand_vars(curr);
	if (ft_strcmp(g_mshell.ls->content, "exit") == 0)
	{
		ft_putstr("exit\n");
		if (g_mshell.ls->next)
			ft_putstr("minishell: exit: too many arguments\n");
		freechar2ptr(g_mshell.env);
		freechar2ptr(g_mshell.vars);
		ft_lstclear(&g_mshell.ls);
		exit(0);
	}
	else if (ft_strcmp(g_mshell.ls->content, "echo") == 0)
		g_mshell.exitcode = echo_ls();
	else if (ft_strcmp(g_mshell.ls->content, "env") == 0)
	{
		env(NULL);
		g_mshell.exitcode = 0;
	}
	else if (ft_strcmp(g_mshell.ls->content, "cd") == 0)
		g_mshell.exitcode = cd();
	else if (ft_strcmp(g_mshell.ls->content, "pwd") == 0)
		g_mshell.exitcode = pwd();
	else if (ft_strcmp(g_mshell.ls->content, "export") == 0)
		g_mshell.exitcode = export(0);
	else if (ft_strcmp(g_mshell.ls->content, "unset") == 0)
		g_mshell.exitcode = unset();
	else if (ft_strcmp(g_mshell.ls->content, "clear") == 0)
		ft_putstr("\033c");
	else
		commandorvar();
	copy = g_mshell.ls;
	int test = 0;
	if (g_mshell.rdirout == 1)
	{
		close(dup(1));
		dup2(g_mshell.oldfdout, 1);
		g_mshell.rdirout = 0;

		// printf("p1 %d p2 %d\n",g_mshell.pipes[2], g_mshell.pipes[5] );
		if(g_mshell.pipes[2] == 0)
		{
			test = 1;
			dup2(g_mshell.pipes[0], 0);
			g_mshell.rdirin = 2;
		}
	}
	if (g_mshell.rdirin == 1)
	{
		close(dup(0));
		dup2(g_mshell.oldfdin, 0);
		g_mshell.rdirin = 0;
	}
	if (test == 0)
		close_pipe_n();
	while (curr)
	{
		if (curr->type == 3 && curr->next != NULL)
		{
			g_mshell.ls = curr->next;
			checkinput_ls();
			break ;
		}
		curr = curr->next;
	}
	g_mshell.ls = copy;
}
