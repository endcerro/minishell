/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:45 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/09 17:17:42 by hpottier         ###   ########.fr       */
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

t_list	*correct_rdir(t_list *lst)
{
	t_list *newlst;
	t_list *curr;
	t_list *prev;
	t_list *block;

	prev = NULL;
	curr = lst;
	newlst = lst;
	while (curr)
	{
		if ((curr->type == 2 || curr->type == 4 || curr->type == 5) && prev)
		{
			block = curr;
			prev->next = (block->next) ? block->next->next : NULL;
			if (block->next)
				block->next->next = NULL;
			addlstendblock(prev, block);
			curr = lst;
		}
		else if ((curr->type == 2 || curr->type == 4 || curr->type == 5) && prev == NULL)
		{
			block = curr;
			curr = NULL;
			if (block->next)
			{
				curr = block->next->next;
				block->next->next = NULL;
			}
			newlst = curr;
			addlstendblock(curr, block);
			curr = newlst;
			continue;
		}
		prev = curr;
		curr = curr->next;
	}
	g_mshell.ls = newlst;
	return (0);
}

int 	islastrdir(t_list *lst, int type)
{
	lst = lst->next;
	while(lst && lst->type != 3 && lst->type != 6)
	{
		if ((type == -5 && lst->type == type) || (type != -5 && (lst->type == -4 || lst->type == - 2)))
			return 0;
		lst = lst->next;
	}
	return 1;
}

int 		trim_rdir(t_list *lst)
{
	t_list *curr;
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
					{
						ft_printh(2, 1, "minishell: %s: no such file or directory\n", curr->next->content);
						return ((g_mshell.exitcode = 1));
					}
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
					{
						ft_printh(2, 1, "minishell: %s: no such file or directory\n", curr->next->content);
						return ((g_mshell.exitcode = 1));
					}
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
					if (fd == -1)
					{
						ft_printh(2, 1, "minishell: %s: no such file or directory\n", curr->next->content);
						return ((g_mshell.exitcode = 1));
					}
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

int		rawtext(t_list *curr)
{
	char **split;
	int i;
	t_list *tmp;
	t_list *cache;
	t_list *prev;

	prev = 0;
	split = 0;
	while (curr)
	{
		if (curr->rawtext == 1)
		{
			i = 0;
			if (prev && (prev->type == 2 || prev->type == 4 || prev->type == 5))
			{
				prev = curr;
				curr = curr->next;
				continue;
			}
			if(!(split = ft_split(curr->content, ' ')))
				return (1);
			while (split && split[i])
			{
				if (i == 0)
				{
					free(curr->content);
					curr->content = split[i];
					// curr->rawtext = 0;
				}
				else
				{
					/* tmp = ft_lstnew(split[i]); */
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
		prev = curr;
		curr = curr->next;
	}
	free(split);
	return (0);
}

int		prep_ls(t_list *curr)
{
	escape_lst(curr);
	if (check_valid(curr) == 0)
		return (2);
	if (expand_vars(curr))
		return (1);
	trimbs(curr);
	if (correctlst(curr))
		return (1);
	if (mergelst(curr))
		return (1);
	if(curr->next && curr->next->next)
		correct_rdir(curr);
	curr = g_mshell.ls;
	if(curr->next && curr->next->next)
		if (trim_rdir(curr))
			return (1);
 	if(rawtext(curr))
 		return (1);
 	t_list *lst = g_mshell.ls;
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

int		openrdir(int *oldfd, int *npipe)
{
	t_list	*urr;
	char	*file;
	int		fd;

	urr = g_mshell.ls;
	while (urr && urr->type != 6)
	{
		if (urr->type == 2 || urr->type == 4 || urr->type == 5)
		{
			if (urr->next)
			{
				if (!(file = urr->next->content))
					return (ft_printh(2, 1, "minishell: %s\n", strerror(errno)));
				if (urr && *npipe > 0)
				{
					if (urr->type == 2)
					{
						if ((fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
							return (ft_printh(2, 1, "minishell: %s\n", strerror(errno)));
						else
							dup2(fd, 1);
					}
					else if (urr->type == 4)
					{
						if ((fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1)
							return (ft_printh(2, 1, "minishell: %s\n", strerror(errno)));
						else
							dup2(fd, 1);
					}
					else if (urr->type == 5)
					{
						if ((fd = open(file, O_RDONLY)) == -1)
							return (ft_printh(2, 1, "minishell: %s: no such file or directory\n", file));
						else
							dup2(fd, 0);
					}
				}
				else if (urr)
				{
					if (urr->type == 2)
					{
						g_mshell.oldfdout = dup(1);
						*oldfd += 1;
						if ((fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
							return (ft_printh(2, 1, "minishell: %s\n", strerror(errno)));
						else
							dup2(fd, 1);
					}
					else if (urr->type == 4)
					{
						g_mshell.oldfdout = dup(1);
						*oldfd += 1;
						if ((fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1)
							return (ft_printh(2, 1, "minishell: %s\n", strerror(errno)));
						else
							dup2(fd, 1);
					}
					else if (urr->type == 5)
					{
						g_mshell.oldfdin = dup(0);
						*oldfd += 2;
						if ((fd = open(file, O_RDONLY)) == -1)
							return (ft_printh(2, 1, "minishell: %s: no such file or directory\n", file));
						else
							dup2(fd, 0);
					}
				}
				urr = urr->next;
			}
		}
		if (urr)
			urr = urr->next;
	}
	return (0);
}

void	exec_command(char *line, t_list *lst, int *npipe)
{
	int		oldfd;
	int		ret;

	oldfd = 0;
	if ((ret = openrdir(&oldfd, npipe)) == 0)
	{
		// rawtext(lst);
		if (ft_strcmp(g_mshell.ls->content, "exit") == 0)
			ms_exit(line, npipe);
		else if (ft_strcmp(g_mshell.ls->content, "echo") == 0)
			g_mshell.exitcode = echo_ls();
		else if (ft_strcmp(g_mshell.ls->content, "env") == 0)
		{
			env(NULL);
			g_mshell.exitcode = 0;
		}
		else if (ft_strcmp(g_mshell.ls->content, "cd") == 0)
			g_mshell.exitcode = cd(0, 0, 0);
		else if (ft_strcmp(g_mshell.ls->content, "pwd") == 0)
			g_mshell.exitcode = pwd();
		else if (ft_strcmp(g_mshell.ls->content, "export") == 0)
			g_mshell.exitcode = export(NULL);
		else if (ft_strcmp(g_mshell.ls->content, "unset") == 0)
			g_mshell.exitcode = unset(NULL);
		else if (ft_strcmp(g_mshell.ls->content, "clear") == 0)
			ft_putstr("\033c");
		else
			commandorvar(npipe, 0);
	}
	if (ret)
		g_mshell.exitcode = ret;
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

int		checkinput_ls(char *line)
{
	t_list	*curr;
	t_list	*copy;
	t_list	*tmp;
	int		x;
	int		pcount;
	int		npipe;
	int		*pipes;
	int		*pidtab;

	if (g_mshell.ls == 0)
		return (1);
	copy = g_mshell.ls;
	tmp = g_mshell.ls;
	pipes = NULL;
	pidtab = NULL;
	while (tmp)
	{
		g_mshell.ls = tmp;
		if ((npipe = prep_ls(tmp)))
			return ((g_mshell.exitcode = npipe));
		// ft_lstprint(tmp);
		curr = g_mshell.ls;
		npipe = countpipes(curr);
		if (npipe != 0)
		{
			if ((pipes = (int *)malloc(sizeof(int) * npipe * 2)) == NULL)
			{
				ft_printh(2, 1, "minishell: %s", strerror(errno));
				g_mshell.ls = copy;
				g_mshell.exitcode = 2;
				return (1);
			}
			if ((pidtab = (int *)malloc(sizeof(int) * (npipe + 1))) == NULL)
			{
				ft_printh(2, 1, "minishell: %s", strerror(errno));
				g_mshell.ls = copy;
				free(pipes);
				g_mshell.exitcode = 2;
				return (1);
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
					free(pidtab);
					g_mshell.exitcode = 2;
					return (1);
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
					free(pipes);
					exec_command(line, copy, &npipe);
				}
				pidtab[x] = g_mshell.pid;
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
			{
				g_mshell.exitcode = 0;
				waitpid(pidtab[x], &g_mshell.exitcode, 0);
				printf("fin exitcode = %d\n", g_mshell.exitcode);
			}
			if (g_mshell.sigswitch != 0)
				g_mshell.exitcode = g_mshell.sigswitch;
			else
				g_mshell.exitcode /= 256;
			g_mshell.sigswitch = 0;
			free(pidtab);
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
	return (0);
}
