/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   morerdir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpottier <hpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:45 by hpottier          #+#    #+#             */
/*   Updated: 2020/08/09 21:11:12 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		openrdir4(t_list **urr, char *file, int *oldfd)
{
	int fd;

	if ((*urr)->type == 2 || (*urr)->type == 4)
	{
		g_mshell.oldfdout = dup(1);
		*oldfd += 1;
		if ((fd = open(file, O_WRONLY | O_CREAT |
			((*urr)->type == 2 ? O_TRUNC : O_APPEND), 0644)) == -1)
			return (ft_printh(2, 1, MERR, strerror(errno)));
		else
			dup2(fd, 1);
	}
	else if ((*urr)->type == 5)
	{
		g_mshell.oldfdin = dup(0);
		*oldfd += 2;
		if ((fd = open(file, O_RDONLY)) == -1)
			return (ft_printh(2, 1, FORDIR, file));
		else
			dup2(fd, 0);
	}
	return (0);
}

int		openrdir2(t_list **urr, char *file, int *npipe, int *oldfd)
{
	if ((*urr) && *npipe > 0)
	{
		if (openrdir3(urr, file) == 1)
			return (1);
	}
	else if ((*urr))
		if (openrdir4(urr, file, oldfd) == 1)
			return (1);
	return (0);
}

int		openrdir(int *oldfd, int *npipe)
{
	t_list	*urr;
	char	*file;

	urr = g_mshell.ls;
	while (urr && urr->type != 6)
	{
		if (urr->type == 2 || urr->type == 4 || urr->type == 5)
		{
			if (urr->next)
			{
				if (!(file = urr->next->content))
					return (ft_printh(2, 1, MERR, strerror(errno)));
				if (openrdir2(&urr, file, npipe, oldfd) == 1)
					return (1);
				urr = urr->next;
			}
		}
		if (urr)
			urr = urr->next;
	}
	return (0);
}

void	exec_command2(int *npipe, int oldfd, char *line, t_list *lst)
{
	if (*npipe <= 0)
	{
		if (oldfd == 2)
			dup2(g_mshell.oldfdin, 0);
		else if (oldfd == 1)
			dup2(g_mshell.oldfdout, 1);
		else if (oldfd == 3)
		{
			dup2(g_mshell.oldfdout, 1);
			dup2(g_mshell.oldfdin, 0);
		}
	}
	if (*npipe > 0)
	{
		free(line);
		freechar2ptr(g_mshell.env, 0);
		freechar2ptr(g_mshell.vars, 0);
		ft_lstclear(&lst);
		exit(g_mshell.exitcode);
	}
}

void	exec_command(char *line, t_list *lst, int *npipe, int oldfd)
{
	int		ret;

	if ((ret = openrdir(&oldfd, npipe)) == 0)
	{
		if (ft_strcmp(g_mshell.ls->content, "exit") == 0)
			ms_exit(line, npipe);
		else if (ft_strcmp(g_mshell.ls->content, "echo") == 0)
			g_mshell.exitcode = echo_ls();
		else if (ft_strcmp(g_mshell.ls->content, "env") == 0)
			g_mshell.exitcode = (int)env(NULL);
		else if (ft_strcmp(g_mshell.ls->content, "cd") == 0)
			g_mshell.exitcode = cd(0, 0, 0);
		else if (ft_strcmp(g_mshell.ls->content, "pwd") == 0)
			g_mshell.exitcode = pwd();
		else if (ft_strcmp(g_mshell.ls->content, "export") == 0)
			g_mshell.exitcode = export(NULL);
		else if (ft_strcmp(g_mshell.ls->content, "unset") == 0)
			g_mshell.exitcode = unset(NULL);
		else
			commandorvar(npipe, 0);
	}
	if (ret)
		g_mshell.exitcode = ret;
	exec_command2(npipe, oldfd, line, lst);
}
