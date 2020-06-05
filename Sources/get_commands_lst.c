/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:45 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/05 19:33:47 by edal--ce         ###   ########.fr       */
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

void	inner_split_loop(t_list *curr, char *buff, int i, int j)
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
		return ;
	free(curr->content);
	curr->content = new->content;
	ft_lstadd_back(&new, curr->next);
	curr->next = new->next;
	free(new);
}

void	inner_split(t_list *lst)
{
	t_list	*curr;
	// t_list	*new;
	// char	**split;
	// int		i;
	// int		j;
	char	buff[2];

	ft_bzero(buff, 2);
	find_char(buff);
	curr = lst;
	while (buff[0])
	{
		while (curr)
		{
			inner_split_loop(curr,buff, -1, 0);
			curr = curr->next;
		}
		find_char(buff);
		curr = lst;
	}
}

t_list	*split_line_lst(char *line)
{
	t_list	*f_lst;
	t_list	*lst;
	int		i;

	i = 0;
	f_lst = 0;
	while ((size_t)i < ft_strlen(line))
	{
		if (ft_isspace(line[i]))
			++i;
		else
		{
			lst = ft_lstnew(get_word_lst(line, &i));
			if (f_lst == 0)
			{
				f_lst = lst;
				lst = 0;
			}
			else
				ft_lstadd_back(&f_lst, lst);
		}
	}
	inner_split(f_lst);
	return (f_lst);
}

void	tag_lst(t_list *lst)
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
}

char	*get_lst(char *line)
{
	char	*filler;
	t_list	*out;

	filler = check_finished_lst(line);
	if (filler)
		line = ft_strjoinft(line, filler);
	out = split_line_lst(line);
	tag_lst(out);
	g_mshell.ls = out;
	g_mshell.ls_b = out;
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

void ft_exit(void)
{
	ft_putstr("exit\n");
	if (g_mshell.ls->next)
		ft_putstr("minishell: exit: too many arguments\n");
	freechar2ptr(g_mshell.env);
	freechar2ptr(g_mshell.vars);
	ft_lstclear(&g_mshell.ls_b);
	exit(0);
}

int is_builtin(char *str)
{
	if (!ft_strcmp(str, "exit") || 
		!ft_strcmp(g_mshell.ls->content, "echo") 	||
		!ft_strcmp(g_mshell.ls->content, "cd")		||
		!ft_strcmp(g_mshell.ls->content, "pwd")		||
		!ft_strcmp(g_mshell.ls->content, "export")	||
		!ft_strcmp(g_mshell.ls->content, "unset")	||
		!ft_strcmp(g_mshell.ls->content, "clear")	||
		!ft_strcmp(g_mshell.ls->content, "env"))
		return (1);
	return (0);
}
int builtin(void)
{
	if(!is_builtin(g_mshell.ls->content))
		return (0);

	//Trim quotes

	if (ft_strcmp(g_mshell.ls->content, "exit") == 0)
		ft_exit();
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
	return (1);
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
	if(!builtin())
		commandorvar();
	copy = g_mshell.ls;
	int test = 0;
	if (g_mshell.rdirout == 1)
	{
		close(dup(1));
		dup2(g_mshell.oldfdout, 1);
		g_mshell.rdirout = 0;
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
