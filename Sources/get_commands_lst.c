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

void	remove_rdir(t_list **lst)
{
	t_list *prev;
	t_list *copy;
	t_list *curr;
	t_list *tmp;

	curr = *lst;
	copy = *lst;
	prev = *lst;
	while (*lst)
	{
		if ((*lst)->type == -1)
		{
			if ((*lst) == copy)
			{
				tmp = (*lst)->next;
				ft_lstdelone((*lst));
				lst = &tmp;
				continue ;
			}
			prev->next = (*lst)->next;
			ft_lstdelone((*lst));
			(*lst) = prev;
		}
		(*lst) = (*lst)->next;
	}
}

void	addlstendblock(t_list *lst, t_list *block)
{
	t_list *prev;

	prev = NULL;
	block->type = -(block->type);
	while (lst)
	{
		if (lst->next && (lst->next->type == 3 || lst->next->type == 6))
		{
			block->next->next = lst->next;
			lst->next = block;
			return ;
		}
		prev = lst;
		lst = lst->next;
	}
	prev->next = block;
}

void	correct_rdir2(t_list **block, t_list **curr, t_list **newlst)
{
	(*block) = (*curr);
	(*curr) = NULL;
	if ((*block)->next)
	{
		(*curr) = (*block)->next->next;
		(*block)->next->next = NULL;
	}
	(*newlst) = (*curr);
	addlstendblock((*curr), (*block));
	(*curr) = (*newlst);
}

t_list	*correct_rdir(t_list *lst, t_list *curr, t_list *newlst, t_list *pr)
{
	t_list *block;

	while (curr)
	{
		if ((curr->type == 2 || curr->type == 4 || curr->type == 5) && pr)
		{
			block = curr;
			pr->next = (block->next) ? block->next->next : NULL;
			if (block->next)
				block->next->next = NULL;
			addlstendblock(pr, block);
			curr = lst;
		}
		else if ((curr->type == 2 || curr->type == 4 || curr->type == 5)
				&& pr == NULL)
		{
			correct_rdir2(&block, &curr, &newlst);
			continue ;
		}
		pr = curr;
		curr = curr->next;
	}
	g_mshell.ls = newlst;
	return (0);
}

int		islastrdir(t_list *lst, int type)
{
	lst = lst->next;
	while (lst && lst->type != 3 && lst->type != 6)
	{
		if ((type == -5 && lst->type == type)
			|| (type != -5 && (lst->type == -4 || lst->type == -2)))
			return (0);
		lst = lst->next;
	}
	return (1);
}

int		rawtext2(t_list **curr, t_list **tmp, t_list **cache, char ***split)
{
	int i;

	i = -1;
	while ((*split) && (*split)[++i])
		if (i == 0)
		{
			free((*curr)->content);
			(*curr)->content = (*split)[i];
		}
		else
		{
			if (!((*tmp) = ft_lstnew((*split)[i])))
				return (freechar2ptr((*split), 1));
			(*split)[i] = (char *)-1;
			(*cache) = (*curr)->next;
			(*tmp)->next = (*cache);
			(*curr)->next = (*tmp);
			(*curr) = (*tmp);
		}
	return (0);
}

int		rawtext(t_list *curr, t_list *prev, char **split, t_list *tmp)
{
	t_list	*cache;

	while (curr)
	{
		if (curr->rawtext == 1)
		{
			if (prev && (prev->type == 2 || prev->type == 4 || prev->type == 5))
			{
				prev = curr;
				curr = curr->next;
				continue ;
			}
			if (!(split = ft_split(curr->content, ' ')))
				return (1);
			if (rawtext2(&curr, &tmp, &cache, &split) == 1)
				return (1);
		}
		prev = curr;
		curr = curr->next;
	}
	free(split);
	return (0);
}

int		prep_ls(t_list *curr, t_list *lst)
{
	escape_lst(curr);
	if (check_valid(curr, 0, 0, curr) != 0)
		return (2);
	if (expand_vars(curr))
		return (1);
	trimbs(curr);
	if (correctlst(curr))
		return (1);
	if (mergelst(curr))
		return (1);
	if (curr->next && curr->next->next)
		correct_rdir(curr, curr, curr, NULL);
	curr = g_mshell.ls;
	if (curr->next && curr->next->next)
		if (trim_rdir(curr))
			return (1);
	curr = g_mshell.ls;
	if (((lst = g_mshell.ls) || 1) && rawtext(curr, NULL, NULL, NULL))
		return (1);
	while (lst && lst->content)
	{
		de_escape_chars(lst->content, -1);
		lst = lst->next;
	}
	return (0);
}

void	ms_exit(char *line, int *npipe)
{
	int ex;

	ex = 0;
	if (*npipe == 0)
		ft_putstr_fd("exit\n", 2);
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

int		openrdir3(t_list **urr, char *file)
{
	int fd;

	if ((*urr)->type == 2 || (*urr)->type == 4)
	{
		if ((fd = open(file, O_WRONLY | O_CREAT |
			((*urr)->type == 2 ? O_TRUNC : O_APPEND), 0644)) == -1)
			return (ft_printh(2, 1, MERR, strerror(errno)));
		else
			dup2(fd, 1);
	}
	else if ((*urr)->type == 5)
	{
		if ((fd = open(file, O_RDONLY)) == -1)
			return (ft_printh(2, 1, FORDIR, file));
		else
			dup2(fd, 0);
	}
	return (0);
}

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

int		countpipes(t_list *curr)
{
	int x;

	x = 0;
	while (curr)
	{
		if (curr->type == 6)
			++x;
		else if (curr->type == 3)
			return (x);
		curr = curr->next;
	}
	return (x);
}

int		allocpipepid(int npipe, t_list *copy)
{
	if (npipe != 0)
	{
		if (((g_mshell.pipes) = (int *)malloc(sizeof(int) * npipe * 2)) == NULL)
		{
			ft_printh(2, 1, "minishell: %s", strerror(errno));
			g_mshell.ls = copy;
			g_mshell.exitcode = 2;
			return (1);
		}
		if (!((g_mshell.pidtab) = (int *)malloc(sizeof(int) * (npipe + 1))))
		{
			ft_printh(2, 1, "minishell: %s", strerror(errno));
			g_mshell.ls = copy;
			free((g_mshell.pipes));
			g_mshell.exitcode = 2;
			return (1);
		}
	}
	return (0);
}

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
