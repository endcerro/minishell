/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:45 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/08 17:37:33 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"









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

int		expand_vars(t_list *lst)			//PROTECTED AND LEAK FREE
{
	t_list *curr;

	curr = lst;
	while (curr && curr->type != 3)
	{
		parse_env_ls(&(curr->content));
		if(curr->content == 0)
		{
			return (1);
		}
		curr = curr->next;
	}
	return (0);
}

int		isstrdigit(char *str)
{
	while (*str)
	{
		if (ft_isdigit(*str) == 0)
			return (0);
		++str;
	}
	return (1);
}

void	checkinput_ls(char *line)
{
	t_list	*curr;
	t_list	*copy;
	int		ex;

	curr = g_mshell.ls;
	if (curr == 0)
		return ;
	check_rdir(curr);
	if(expand_vars(curr))
	{
		return ;
	}
	if (ft_strcmp(g_mshell.ls->content, "exit") == 0)
	{
		ft_putstr("exit\n");
		ex = 0;
		if (g_mshell.ls->next)
		{
			if (isstrdigit(g_mshell.ls->next->content) == 0)
				ft_putstr("minishell: exit: numeric argument needed\n");
			else
				ex = ft_atoi(g_mshell.ls->next->content);
		}
		freechar2ptr(g_mshell.env);
		freechar2ptr(g_mshell.vars);
		ft_lstclear(&g_mshell.ls);
		free(line);
		exit(ex);
	}
	else if (ft_strcmp(g_mshell.ls->content, "echo") == 0)
		g_mshell.exitcode = echo_ls();						//PROTECTED
	else if (ft_strcmp(g_mshell.ls->content, "env") == 0) //PROTECTED UNTILL HERE
	{
		env(NULL);
		g_mshell.exitcode = 0;
	}
	else if (ft_strcmp(g_mshell.ls->content, "cd") == 0)
		g_mshell.exitcode = cd();							//CHECK PROTECTION
	else if (ft_strcmp(g_mshell.ls->content, "pwd") == 0)
		g_mshell.exitcode = pwd();
	else if (ft_strcmp(g_mshell.ls->content, "export") == 0)
		g_mshell.exitcode = export(0);							//PROTECTED
	else if (ft_strcmp(g_mshell.ls->content, "unset") == 0)
		g_mshell.exitcode = unset();							//PROTECTED
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
			checkinput_ls(line);
			break ;
		}
		curr = curr->next;
	}
	g_mshell.ls = copy;
}
