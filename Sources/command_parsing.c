/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:49 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/11 22:47:12 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*add_filler(char **fill)
{
	int		i;
	char	*out;

	out = 0;
	i = 0;
	if (fill && fill[i])
	{
		if ((out = ft_strdup(fill[i])) == NULL)
			return ((char *)(long)freechar2ptr(fill, 0));
		while (fill[++i])
		{
			if ((out = ft_strjoinf1(out, "\n")) == NULL)
				break ;
			if ((out = ft_strjoinf1(out, fill[i])) == NULL)
				break ;
		}
	}
	return (out);
}

char	*check_finished_lst(char *line, int *err)
{
	char	cpt[3];
	char	**fill;
	char	*out;

	fill = 0;
	out = 0;
	ft_bzero(cpt, 3);
	escape_chars(line, 0, 0);
	parse_qts(line, cpt);
	if (cpt[0] % 2 || cpt[1] % 2 || cpt[2])
		if ((fill = getfiller(0, cpt, err)) == NULL)
			return (NULL);
	if ((out = add_filler(fill)) == NULL)
		return (NULL);
	freechar2ptr(fill, 0);
	return (out);
}

int		rdirout(t_list *curr, int mode)
{
	int		fd;

	if (curr->next == 0 || curr->next->type != 1)
		return (1);
	if (g_mshell.rdirout == 1)
	{
		g_mshell.rdirout = 0;
		if (close(1) == -1)
			ft_putstr_fd("ERROR CLOSING FD", 2);
	}
	if (mode == 0)
		fd = open(curr->next->content, O_APPEND | O_TRUNC | O_WRONLY | O_CREAT
			, 0644);
	else
		fd = open(curr->next->content, O_APPEND | O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		close(fd);
		errno = 1;
		ft_printh(2, 1, "%s: No such file or directory\n", curr->next->content);
		return (1);
	}
	dup2(fd, 1);
	g_mshell.rdirout = 1;
	return (0);
}

int		check_rdir_loop(t_list *curr)
{
	int		fd;

	if ((curr->type == 2 || curr->type == 4)
		&& rdirout(curr, (curr->type == 4)))
		return (1);
	else if (curr->type == 5 && curr->next && curr->next->type == 1)
	{
		if (g_mshell.rdirin == 1)
			if (close(0) == -1)
				ft_putstr_fd("ERROR CLOSING FD", 2);
		g_mshell.rdirin = 1;
		if ((fd = open(curr->next->content, O_RDONLY)) == -1)
		{
			close(fd);
			ft_printh(2, 1, "%s: No such file or directory\n",
				curr->next->content);
			g_mshell.rdirin = 0;
			return (1);
		}
		else
			dup2(fd, 0);
	}
	else if (curr->type == 6 && curr->next && curr->next->type == 1)
		return (open_pipe_n(curr));
	return (2);
}

int		check_rdir(t_list *curr)
{
	int		ret;

	while (curr && curr->type != 3)
	{
		ret = check_rdir_loop(curr);
		if (ret != 2)
			return (ret);
		curr = curr->next;
	}
	return (0);
}
