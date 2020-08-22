/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingdeouf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:45 by hpottier          #+#    #+#             */
/*   Updated: 2020/08/22 15:04:37 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int		prep_ls(t_list *curr, t_list *lst, int ret)
{
	ft_lstprint(curr);
	escape_lst(curr);
	if (check_valid(curr, 0, 0, curr) != 0)
		return (2);
	if ((ret = expand_vars(curr)) != -1)
		return (ret);
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
	return (-1);
}

int		ms_exit(char *line, int *npipe)
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
			return (1);
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
