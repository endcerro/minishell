/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:45 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/09 21:11:12 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	checkinput_ls3(int *npipe, char *line, t_list *copy, int *xpcount)
{
	if (g_mshell.pid == 0)
	{
		if (*npipe > xpcount[0])
			dup2(g_mshell.pipes[xpcount[1] + 1], 1);
		if (xpcount[0] > 0)
			dup2(g_mshell.pipes[xpcount[1] - 2], 0);
		xpcount[0] = -1;
		while (++xpcount[0] < *npipe * 2)
			close(*(g_mshell.pipes + xpcount[0]));
		free(g_mshell.pipes);
		exec_command(line, copy, npipe, 0);
	}
	g_mshell.pidtab[xpcount[0]] = g_mshell.pid;
	while (g_mshell.ls)
	{
		if (g_mshell.ls->type == 6
			&& ((g_mshell.ls = g_mshell.ls->next) && 1))
			break ;
		g_mshell.ls = g_mshell.ls->next;
	}
}

int		checkinput_ls2(t_list *copy, int *npipe, char *line)
{
	int xpcount[2];

	xpcount[0] = -1;
	xpcount[1] = -2;
	while (((++xpcount[0] | (xpcount[1] += 2)) && 0) || g_mshell.ls)
	{
		if ((g_mshell.pid = fork()) == -1)
		{
			ft_printh(2, 1, "minishell: %s", strerror(errno));
			g_mshell.ls = copy;
			xpcount[0] = -1;
			while (++xpcount[0] < *npipe * 2)
				close(*(g_mshell.pipes + xpcount[0]));
			free(g_mshell.pipes);
			free(g_mshell.pidtab);
			g_mshell.exitcode = 2;
			return (1);
		}
		checkinput_ls3(npipe, line, copy, xpcount);
	}
	return (0);
}

int		checkinput_ls4(int *npipe, t_list *copy, char *line, int x)
{
	if (*npipe != 0)
	{
		x = -2;
		while ((x += 2) < *npipe * 2)
			pipe(g_mshell.pipes + x);
		if (checkinput_ls2(copy, npipe, line) == 1)
			return (1);
		x = -1;
		while (++x < *npipe * 2)
			close(*(g_mshell.pipes + x));
		free(g_mshell.pipes);
		g_mshell.pipes = NULL;
		x = -1;
		while (++x <= *npipe)
			waitpid(g_mshell.pidtab[x], &g_mshell.exitcode, 0);
		if (g_mshell.sigswitch != 0)
			g_mshell.exitcode = g_mshell.sigswitch;
		else
			g_mshell.exitcode /= 256;
		g_mshell.sigswitch = 0;
		free(g_mshell.pidtab);
	}
	else
		exec_command(line, copy, npipe, 0);
	return (0);
}

int		checkinput_ls5(t_list **tmp, t_list **curr, int *npipe, t_list **copy)
{
	if (*tmp == *copy)
		*copy = g_mshell.ls;
	*tmp = g_mshell.ls;
	*curr = g_mshell.ls;
	*npipe = countpipes(*curr);
	if (allocpipepid(*npipe, *copy) == 1)
		return (1);
	while (*curr && (*curr)->type != 3)
		*curr = (*curr)->next;
	return (0);
}

int		checkinput_ls(char *line, t_list *tmp, t_list *copy, int npipe)
{
	t_list	*curr;

	if (g_mshell.ls == 0)
		return (1);
	while (tmp && (g_mshell.ls = tmp))
	{
		if ((npipe = prep_ls(tmp, NULL)))
			return ((g_mshell.exitcode = npipe));
		if (checkinput_ls5(&tmp, &curr, &npipe, &copy) == 1)
			return (1);
		if (curr && ((tmp = curr->next) && 1))
			curr->next = NULL;
		else
			tmp = NULL;
		curr = g_mshell.ls;
		if (checkinput_ls4(&npipe, copy, line, 0) == 1)
			return (1);
		g_mshell.pid = 0;
		while (curr->next)
			curr = curr->next;
		curr->next = tmp;
	}
	g_mshell.ls = copy;
	return (0);
}
