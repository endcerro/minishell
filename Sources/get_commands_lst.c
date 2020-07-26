/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:45 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/26 17:34:23 by edal--ce         ###   ########.fr       */
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

void 	addlstendblock(t_list *lst, t_list *block)
{
	t_list *prev;
	prev = 0;
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

t_list *correct_rdir(t_list *lst)
{
	t_list *newlst;
	t_list *curr;
	t_list *prev;
	t_list *block;

	// ft_lstprint(lst);
	prev = 0;
	curr = lst;
	newlst = lst;
	while (curr)
	{
		if( (curr->type == 2 || curr->type == 4 || curr->type == 5) && prev)
		{
			block = curr;
			prev->next = block->next->next;
			block->next->next = 0;
			addlstendblock(prev, block);
			curr = lst;
		}
		else if ((curr->type == 2 || curr->type == 4 || curr->type == 5) && prev == 0)
		{
			block = curr;
			curr = block->next->next;
			newlst = curr;
			block->next->next = 0;
			addlstendblock(curr, block);
			curr = newlst;
			continue;
		}
		prev = curr;
		curr = curr->next;
	}
	// ft_lstprint(newlst);
	g_mshell.ls = newlst;
	return (0);
}

int 	islastrdir(t_list *lst, int type)
{
	lst = lst->next;
	while(lst && lst->type != 3 && lst->type != 6)
	{
		// printf("Loop\n");
		if ((type == -5 && lst->type == type) || (type != -5 && (lst->type == -4 || lst->type == - 2)))
			return 0;
		lst = lst->next;
	}
	return 1;
}

int 	trim_rdir(t_list *lst)
{
	t_list *curr;
	t_list *tmp;
	t_list *prev;
	int fd;

	curr = lst;
	prev = 0;
	while (curr && curr->type != 3)
	{
		while (curr && curr->type != 6)
		{
			if (curr->type == -2 )
			{
				curr->type = 2;
				if (curr->next->next && !islastrdir(curr, -2))
				{
					fd = open(curr->next->content, O_APPEND | O_TRUNC | O_WRONLY | O_CREAT
				, 0644);
					if (fd == -1)
						return (1);
					close(fd);
					prev->next = curr->next->next;
					ft_lstdelone(curr->next);
					ft_lstdelone(curr);
					curr = prev;
					continue ;
				}
			}
			else if (curr->type == -4)
			{
				curr->type = 4;
				if (curr->next->next && !islastrdir(curr, -4))
				{
					fd = open(curr->next->content, O_APPEND | O_WRONLY | O_CREAT
				, 0644);
					if (fd == -1)
						return (1);
					close(fd);
					prev->next = curr->next->next;
					ft_lstdelone(curr->next);
					ft_lstdelone(curr);
					curr = prev;
					continue ;
				}
			}
			else if (curr->type == -5)
			{
				curr->type = 5;
				if (curr->next->next && !islastrdir(curr, -5))
				{
					int fd;

					fd = open(curr->next->content, O_RDONLY);
					if(fd == -1)
						return (1);
					close(fd);
					prev->next = curr->next->next;
					ft_lstdelone(curr->next);
					ft_lstdelone(curr);
					curr = prev;
					continue ;
				}
			}
			prev = curr;
			curr = curr->next;
		}
		prev = curr;
		curr = (curr) ? curr->next : 0;
	}
	return (0);
}

int rawtext(t_list *curr)
{
	char **split;
	int i;
	t_list *tmp;
	t_list *cache;

	split = 0;
	while (curr)
	{
		if (curr->rawtext == 1)
		{
			i = 0;
			if(!(split = ft_split(curr->content, ' ')))
				return (1);	
			while (split[i])
			{
				if (i == 0)
				{
					free(curr->content);
					curr->content = split[i];
					curr->rawtext = 0;
				}
				else
				{
					tmp = ft_lstnew(split[i]);
					if (!(tmp = ft_lstnew(split[i])))
						return (freechar2ptr(split, 1));
					split[i] = (char *)-1;
					cache = curr->next;
					tmp->next = cache;
					curr->next = tmp;
					curr = tmp;
				}
				i++;
			}
		}
		curr = curr->next;
	}
	free(split);
	return (0);
}

int		prep_ls(t_list *curr)
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

	correct_rdir(curr);
	curr = g_mshell.ls;
	
	if (trim_rdir(curr))
		return (1); //HANDLE FD ERRRORS

	if(rawtext(curr))
		return (1);


	// trim_rdir(*curr);
	/* if (check_rdir(curr) == 1) */
	/* 	return (1); */
	/* remove_rdir(&curr); */
	return (0);
}

void	ms_exit(char *line)
{
	int ex;

	ex = 0;
	if (line != NULL)
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

void	exec_command(char *line, t_list *lst, int *npipe)
{
	t_list	*urr;
	char	*file;
	int		fd;
	int		oldfd;

	urr = g_mshell.ls;
	file = NULL;
	oldfd = 0;
	while (urr && urr->type != 6)
	{
		if (urr->type == 2 || urr->type == 4 || urr->type == 5)
		{
			if (urr->next)
			{
				file = urr->next->content;
				if (file && urr && *npipe > 0)
				{
					if (urr->type == 2)
					{
						if ((fd = open(file, O_WRONLY | O_CREAT | O_TRUNC)) == -1)
							ft_printh(2, 1, "minishell: %s\n", strerror(errno));
						else
							dup2(fd, 1);
					}
					else if (urr->type == 4)
					{
						if ((fd = open(file, O_WRONLY | O_CREAT | O_APPEND)) == -1)
							ft_printh(2, 1, "minishell: %s\n", strerror(errno));
						else
							dup2(fd, 1);
					}
					else if (urr->type == 5)
					{
						if ((fd = open(file, O_RDONLY)) == -1)
							ft_printh(2, 1, "minishell: %s: no such file or directory\n", file);
						else
							dup2(fd, 0);
					}
				}
				else if (file && urr)
				{
					if (urr->type == 2)
					{
						g_mshell.oldfdout = dup(1);
						++oldfd;
						if ((fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
							ft_printh(2, 1, "minishell: %s\n", strerror(errno));
						else
							dup2(fd, 1);
					}
					else if (urr->type == 4)
					{
						g_mshell.oldfdout = dup(1);
						++oldfd;
						if ((fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1)
							ft_printh(2, 1, "minishell: %s\n", strerror(errno));
						else
							dup2(fd, 1);
					}
					else if (urr->type == 5)
					{
						g_mshell.oldfdin = dup(0);
						oldfd += 2;
						if ((fd = open(file, O_RDONLY)) == -1)
							ft_printh(2, 1, "minishell: %s: no such file or directory\n", file);
						else
							dup2(fd, 0);
					}
				}
				urr = urr->next;
			}
		}
		urr = urr->next;
	}
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
		g_mshell.exitcode = export(NULL);
	else if (ft_strcmp(g_mshell.ls->content, "unset") == 0)
		g_mshell.exitcode = unset();
	else if (ft_strcmp(g_mshell.ls->content, "clear") == 0)
		ft_putstr("\033c");
	else
		commandorvar(npipe);
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

/* void	prep_rdir(int ex) */
/* { */
/* 	if (g_mshell.rdirout == 1) */
/* 	{ */
/* 		if (close(dup(1)) == -1) */
/* 			ft_putstr_fd("ERROR CLOSING FD", 2); */
/* 		dup2(g_mshell.oldfdout, 1); */
/* 		g_mshell.rdirout = 0; */
/* 		if (g_mshell.pipes[2] == 0) */
/* 		{ */
/* 			if (g_mshell.rdirin != 2) */
/* 			{ */
/* 				ex = 1; */
/* 				dup2(g_mshell.pipes[0], 0); */
/* 				g_mshell.rdirin = 2; */
/* 			} */
/* 		} */
/* 	} */
/* 	if (g_mshell.rdirin == 1) */
/* 	{ */
/* 		if (close(dup(0)) == -1) */
/* 			ft_putstr_fd("ERROR CLOSING FD\n", 2); */
/* 		dup2(g_mshell.oldfdin, 0); */
/* 		g_mshell.rdirin = 0; */
/* 	} */
/* 	if (ex == 0) */
/* 		close_pipe_n(); */
/* } */

int		check_exit(char *line) // rejouter redirections
{
	t_list *curr;

	if (ft_strcmp(g_mshell.ls->content, "exit") == 0)
	{
		curr = g_mshell.ls;
		while (curr)
		{
			if (curr->type == 6)
				return (0);
			curr = curr->next;
		}
		ms_exit(line);
		return (1);
	}
	return (0);
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

void	checkinput_ls(char *line)
{
	t_list	*curr;
	t_list	*copy;
	t_list	*tmp;
	int		x;
	int		pcount;
	int		npipe;
	int		*pipes;

	if (g_mshell.ls == 0)
		return ;
	// if (prep_ls(g_mshell.ls))
	// 	return ;
	// ft_lstprint(g_mshell.ls);
	if (check_exit(line))
		return ;
	copy = g_mshell.ls;
	tmp = g_mshell.ls;
	pipes = NULL;
	while (tmp)
	{
		g_mshell.ls = tmp;
		if (prep_ls(tmp))
			return ;
		ft_lstprint(tmp);
		curr = g_mshell.ls;
		npipe = countpipes(curr);
		if (npipe != 0)
		{
			if ((pipes = (int *)malloc(sizeof(int) * npipe * 2)) == NULL)
			{
				ft_printh(2, 1, "minishell: %s", strerror(errno));
				g_mshell.ls = copy;
				g_mshell.exitcode = 2;
				return ;
			}
		}
		while (curr && curr->type != 3)
			curr = curr->next;
		if (curr)
		{
			tmp = curr->next;
			curr->next = NULL;
		}
		else
			tmp = NULL;
		curr = g_mshell.ls;
		if (npipe != 0)
		{
			x = 0;
			while (x < npipe * 2)
			{
				pipe(pipes + x);
				x += 2;
			}
			x = 0;
			pcount = 0;
			while (g_mshell.ls)
			{
				if ((g_mshell.pid = fork()) == -1)
				{
					ft_printh(2, 1, "minishell: %s", strerror(errno));
					g_mshell.ls = copy;
					x = -1;
					while (++x < npipe * 2)
						close(*(pipes + x));
					free(pipes);
					g_mshell.exitcode = 2;
					return ;
				}
				if (g_mshell.pid == 0)
				{
					if (npipe > x)
						dup2(pipes[pcount + 1], 1);
					if (x > 0)
						dup2(pipes[pcount - 2], 0);
					x = -1;
					while (++x < npipe * 2)
						close(*(pipes + x));
					exec_command(line, copy, &npipe);
				}
				while (g_mshell.ls)
				{
					if (g_mshell.ls->type == 6)
					{
						g_mshell.ls = g_mshell.ls->next;
						break ;
					}
					g_mshell.ls = g_mshell.ls->next;
				}
				pcount += 2;
				++x;
			}
			x = -1;
			while (++x < npipe * 2)
				close(*(pipes + x));
			free(pipes);
			pipes = NULL;
			x = -1;
			while (++x <= npipe)
				wait(&g_mshell.exitcode);
			if (g_mshell.sigswitch != 0)
				g_mshell.exitcode = g_mshell.sigswitch;
			else
				g_mshell.exitcode /= 256;
			g_mshell.sigswitch = 0;
		}
		else
		{
			exec_command(line, copy, &npipe);
		}
		g_mshell.pid = 0;
		while (curr->next)
			curr = curr->next;
		curr->next = tmp;
	}
	g_mshell.ls = copy;
}
