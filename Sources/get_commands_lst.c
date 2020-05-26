/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:45 by edal--ce          #+#    #+#             */
/*   Updated: 2020/05/26 16:36:37 by edal--ce         ###   ########.fr       */
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
	return (f_lst);
}

void tag_lst(t_list *lst)
{
	t_list *curr;

	curr = lst;
	while (curr)
	{
		if (ft_strcmp(curr->content, ">") == 0)	//REDIRECTION ARE 3
			curr->type = 2;
		else if (ft_strcmp(curr->content, ";") == 0) //END OF COMMAND ARE 2
			curr->type = 3;
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
	tag_lst(out);
	g_mshell.ls = out;
}

void	echo_ls()	//Devrait etre pas mal, à vérifier
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
}

// void check_rdir()
// {
// 	t_list *curr;
// 	int fd;

// 	curr = g_mshell.ls;

// 	while(curr && curr->type != 3)
// 	{
// 		if (curr->type == 2 && curr->next && curr->next->type == 1)
// 		{
// 			// printf("oldfd = %d\n",dup(1));
// 			g_mshell.oldfd = dup(1);
// 			fd = open(curr->next->content, O_APPEND | O_TRUNC | O_WRONLY | O_CREAT, 0644);
// 			// printf("newfd = %d\n",fd);
// 			dup2(fd, 1);
// 		}
// 		curr = curr->next;
// 	}
// }

void	checkinput_ls(void)
{
	// printf("CHECKING INPUT %s\n",g_mshell.params[0] );
	// check_command(0);
	t_list *curr;

	curr = g_mshell.ls;
	g_mshell.oldfd = 0;
	check_rdir();

	if (ft_strcmp(g_mshell.ls->content, "exit") == 0) // Fini
	{
		ft_putstr("exit\n");
		if (g_mshell.ls->next)
			ft_putstr("minishell: exit: too many arguments\n");
		freechar2ptr(g_mshell.env);
		// freechar2ptr(g_mshell.params);
		freechar2ptr(g_mshell.vars);
		exit(0);
	}
	else if (ft_strcmp(g_mshell.ls->content, "echo") == 0) // A terminer
	{
		// printf("echo ls\n");
		echo_ls();
	}
	else if (ft_strcmp(g_mshell.ls->content, "env") == 0) // Fini
	{
		// printf("env ls\n");
		env(NULL);
	}
	else if (ft_strcmp(g_mshell.ls->content, "cd") == 0) // Fini // PAS PROTEGE
		cd();
	else if (ft_strcmp(g_mshell.ls->content, "pwd") == 0) // Fini
		pwd();
	else if (ft_strcmp(g_mshell.ls->content, "export") == 0) // A fignoler // PAS PROTEGE
		export(0);
	else if (ft_strcmp(g_mshell.ls->content, "unset") == 0) // A terminer
		unset();
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
	while(curr)
	{
		if(curr->type == 3 && curr->next != NULL)
		{
			g_mshell.ls = curr->next;
			checkinput_ls();
		}
		curr = curr->next;
	}
	g_mshell.ls = copy;

}
