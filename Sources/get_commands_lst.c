/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:45 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/09 18:01:00 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		expand_vars(t_list *curr)					//PROTECTED
{
	t_list *prev;

	prev = 0;
	while (curr && curr->type != 3 && curr->type != 6)
	{
		if (curr->content[0] != '\'')
		{
			parse_env_ls(&(curr->content));
			if (curr->content[0] == 0)
			{
				if (prev)
				{
					prev->next = curr->next;
					ft_lstdelone(curr);
				}
				curr = prev;
			}
		}
		if (curr->content == 0)
			return (1);
		prev = curr;
		curr = curr->next;
	}
	return (0);
}

int		isstrdigit(char *str)						//PROTECTED
{
	while (*str && (*str == '-' || *str == '+'))
		++str;
	while (*str)
	{
		if (ft_isdigit(*str) == 0)
			return (0);
		++str;
	}
	return (1);
}

void	deconechar(char *str)						//PROTECTED
{
	int i;

	i = 0;
	if (*str == 0)
		return ;
	while (str[++i])
		str[i - 1] = str[i];
	str[i - 1] = str[i];
}

void	trimbs_loop(t_list *curr, int i)
{
	int j;
	int qtcp;

	while (curr->content[i])
	{
		if (curr->content[i] == '\\')
		{
			qtcp = 0;
			j = i - 1;
			while (curr->content[++j] == '\\')
				qtcp++;
			if ((qtcp != 1 && qtcp % 2 == 0) || curr->content[0] == '\"')
				qtcp /= 2;
			else if (qtcp != 1)
				qtcp = (qtcp - 1) / 2 + 1;
			if (curr->content[0] == '\"' && curr->content[j] < 0)
				qtcp++;
			j = -1;
			while (++j < qtcp)
				deconechar(curr->content + i);
			i += qtcp;
		}
		else
			i++;
	}
}

void	trimbs(t_list *curr)					//PROTECTED
{
	while (curr && curr->content && curr->type == 1)
	{
		if (curr->content[0] != '\'')
			trimbs_loop(curr, 0);
		curr = curr->next;
	}
}

int		check_valid(t_list *lst)				//PROTECTED
{
	int cp_r;
	int cp_d;

	cp_r = 0;
	cp_d = 0;
	while (lst)
	{
		if (lst->type == 3 || lst->type == 6)
			cp_d++;
		else if (lst->type != 1)
			cp_r++;
		else
		{
			cp_r = 0;
			cp_d = 0;
		}
		if ((cp_d > 1 || cp_r > 1))
			return (ft_printh(2, 0, SYNTERR, lst->content));
		lst = lst->next;
	}
	return (1);
}

int		prep_ls(t_list *curr)				//PROTECTED
{
	escape_lst(curr);
	if (check_valid(curr) == 0)
		return (1);
	if (expand_vars(curr))
		return (1);
	trimbs(curr);
	if (correctlst(curr))
		return (1);
	if (mergelst(curr))
		return (1);
	if (check_rdir(curr) == 1)
		return (1);
	return (0);
}

void	ms_exit(char *line)
{
	int ex;

	ex = 0;
	ft_putstr("exit\n");
	if (g_mshell.ls->next && g_mshell.ls->next->type == 1)
	{
		if (isstrdigit(g_mshell.ls->next->content) == 0)
			ft_printh(2, 0, "minishell: exit: %s: numeric argument needed\n",
					g_mshell.ls->next->content);
		else if (g_mshell.ls->next->next && g_mshell.ls->next->next->type == 1)
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return ;
		}
		else
			ex = ft_atoi(g_mshell.ls->next->content);
	}
	freechar2ptr(g_mshell.env, 0);
	freechar2ptr(g_mshell.vars, 0);
	ft_lstclear(&g_mshell.ls);
	free(line);
	exit(ex);
}

void	exec_command(char *line)
{
	if (ft_strcmp(g_mshell.ls->content, "exit") == 0)
		ms_exit(line);
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
}

void	prep_rdir(int ex)
{
	if (g_mshell.rdirout == 1)
	{
		if (close(dup(1)) == -1)
			ft_putstr_fd("ERROR CLOSING FD", 2);
		dup2(g_mshell.oldfdout, 1);
		g_mshell.rdirout = 0;
		if (g_mshell.pipes[2] == 0)
		{
			if (g_mshell.rdirin != 2)
			{
				ex = 1;
				dup2(g_mshell.pipes[0], 0);
				g_mshell.rdirin = 2;
			}
		}
	}
	if (g_mshell.rdirin == 1)
	{
		if (close(dup(0)) == -1)
			ft_putstr_fd("ERROR CLOSING FD\n", 2);
		dup2(g_mshell.oldfdin, 0);
		g_mshell.rdirin = 0;
	}
	if (ex == 0)
		close_pipe_n();
}

void	checkinput_ls(char *line)
{
	t_list	*curr;
	t_list	*copy;
	int		ex;

	curr = g_mshell.ls;
	if (g_mshell.ls == 0)
		return ;
	if (prep_ls(g_mshell.ls))
		return ;
	exec_command(line);
	copy = g_mshell.ls;
	ex = 0;
	prep_rdir(0);
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
