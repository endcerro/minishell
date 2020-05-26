/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 15:28:59 by edal--ce          #+#    #+#             */
/*   Updated: 2020/05/26 15:34:40 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

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

void	get_lst(char *line)
{
	char 	*filler;
	t_list 	*out;

	filler = check_finished_lst(line);
	if (filler)
	{
		line = ft_strjoinft(line, filler);
	}
	parse_env_ls(&line);
	out = split_line_lst(line);
	tag_lst(out);
	tag_lst(out);
	g_mshell.ls = out;
}

void	echo_ls()	//Devrait etre pas mal, à vérifier
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
			write(1," ",1);
		curr = curr->next;
	}
	if (ret)
		write(1, "\n", 1);
}

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
		export(g_mshell.params);
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
