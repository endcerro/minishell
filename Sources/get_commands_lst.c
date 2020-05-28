/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:45 by edal--ce          #+#    #+#             */
/*   Updated: 2020/05/28 15:16:35 by edal--ce         ###   ########.fr       */
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
		return(ft_substr(line, cp, len + 2));
	}
	else if (line[*p] == '\'' && ++(*p))
	{
		while (line[*p] && line[*p] != '\'' && ++len)
			++(*p);
		++(*p);
		return(ft_substr(line, cp, len + 2));
	}
	else
		while (line[*p] && !ft_isspace(line[*p]) && ++len)
			++(*p);
	return(ft_substr(line, cp, len));
}

void find_char(char *buff)
{
	if (buff[0] == 0)
		buff[0] = ';';
	else if (buff[0] == ';')
		buff[0] = '>';
	else if (buff[0] == '>')
		buff[0] = '<';
	else if (buff[0] == '<')
		buff[0] = 0;
	// else if (buff[0] == '|')
	// 	buff[0] = '>';
}

void inner_split(t_list *lst)
{
	t_list 	*curr;
	t_list 	*new;
	char 	**split;
	int 	i;
	char 	*pos;

	char 	buff[2];
	ft_bzero(buff,2);
	find_char(buff);
	curr = lst;
	while(buff[0])
	{	
		while(curr)
		{
			new = 0;
			if(curr->content[0] != '\'' && curr->content[0] != '\"' && curr->content[1])
			{
				pos = 0;
				if(ft_strchr(curr->content, buff[0]))
				{
					pos = ft_strchr(curr->content, buff[0]);
					split = ft_split(curr->content, buff[0]);
					i = -1;
					while (split[++i])
					{
						if (pos == curr->content)
						{
							ft_lstadd_back(&new, ft_lstnew(ft_strdup(buff)));	
							pos = ft_strchr(pos + 1, buff[0]);
						}
						ft_lstadd_back(&new, ft_lstnew(split[i]));
						if (pos != 0)
						{
							ft_lstadd_back(&new, ft_lstnew(ft_strdup(buff)));
							pos = ft_strchr(pos + 1, buff[0]);
						}
					}
					free(split);
				}
			}
			if(new)
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
}

t_list *split_line_lst(char *line)
{
	t_list 	*f_lst;
	t_list 	*lst;
	int 	i;

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
//	ft_lstprint(f_lst);
	inner_split(f_lst);
	return (f_lst);
}

void tag_lst(t_list *lst)
{
	t_list *curr;

	curr = lst;
	while (curr)
	{
		if (ft_strcmp(curr->content, ">") == 0)	//REDIRECTION ARE 2
			curr->type = 2;
		else if (ft_strcmp(curr->content, ";") == 0) //END OF COMMAND ARE 3
			curr->type = 3;
		else if (ft_strcmp(curr->content, ">>") == 0) //APPEND RDIR ARE 4
			curr->type = 4;
		else if (ft_strcmp(curr->content, "<") == 0) //INPUT RDIR ARE 5
			curr->type = 5;
		curr = curr->next;
	}
}

void	get_lst(char *line)
{
	char *filler;

	filler = check_finished_lst(line);

	if (filler)
	{
		// printf("here\n");
		line = ft_strjoinft(line, filler);
	}

	// printf("Line b4 env= %s\n",line );
	parse_env_ls(&line);
	// printf("Line af env= %s\n",line );


	t_list *out = split_line_lst(line);
	tag_lst(out);
	// free(line);
	/* ft_lstprint(out); */
	/* ft_putstr("\n\n"); */
//	ft_lstprint(out);
	// tag_lst(out);
	g_mshell.ls = out;
}

int		echo_ls(void)	//Devrait etre pas mal, à vérifier
{
	int		ret;
	t_list	*curr;

	ret = 0;
	// fill = 0;
	curr = g_mshell.ls->next;
	if (curr && ft_strcmp(curr->content, "-n") == 0)
		curr = curr->next;
	else
		ret = 1;
	while (curr && curr->type == 1)
	{
		ft_putstr(curr->content);
		if (curr->next)
			write(1," ",1);
		curr = curr->next;
	}
	if (ret)
		write(1, "\n", 1);
	return (0);
}

void	checkinput_ls(void)
{
	// printf("CHECKING INPUT %s\n",g_mshell.params[0] );
	// check_command(0);
	t_list *curr;

	curr = g_mshell.ls;
	g_mshell.oldfd = 0;
	g_mshell.oldfd_in = 0;
	check_rdir();

	if (ft_strcmp(g_mshell.ls->content, "exit") == 0) // Fini
	{
		ft_putstr("exit\n");
		if (g_mshell.ls->next)
			ft_putstr("minishell: exit: too many arguments\n");
		freechar2ptr(g_mshell.env);
		freechar2ptr(g_mshell.vars);
		exit(0);
	}
	else if (ft_strcmp(g_mshell.ls->content, "echo") == 0) // A terminer
		g_mshell.exitcode = echo_ls();
	else if (ft_strcmp(g_mshell.ls->content, "env") == 0) // Fini
	{
		env(NULL);
		g_mshell.exitcode = 0;
	}
	else if (ft_strcmp(g_mshell.ls->content, "cd") == 0) // Fini // PAS PROTEGE
		g_mshell.exitcode = cd();
	else if (ft_strcmp(g_mshell.ls->content, "pwd") == 0) // Fini
		g_mshell.exitcode = pwd();
	else if (ft_strcmp(g_mshell.ls->content, "export") == 0) // A fignoler // PAS PROTEGE
		g_mshell.exitcode = export(0);
	else if (ft_strcmp(g_mshell.ls->content, "unset") == 0) // A terminer
		g_mshell.exitcode = unset();
	else if (ft_strcmp(g_mshell.ls->content, "clear") == 0)
		ft_putstr("\033c");
	else
		commandorvar();


	t_list *copy;
	copy = g_mshell.ls;
	if (g_mshell.oldfd != 0)
	{
		close(dup(1));
		dup2(g_mshell.oldfd, 1);
		g_mshell.oldfd = 0;
	}
	if (g_mshell.oldfd_in != 0)
	{
		// printf("here\n");
		close(dup(0));
		dup2(g_mshell.oldfd_in, 0);
		g_mshell.oldfd_in = 0;
	}
	while(curr)
	{
		if(curr->type == 3 && curr->next != NULL)
		{
			g_mshell.ls = curr->next;
			checkinput_ls();
			g_mshell.ls = copy;
			return ;
		}
		curr = curr->next;
	}
	g_mshell.ls = copy;
}
