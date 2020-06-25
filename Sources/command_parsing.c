/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:49 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/25 10:53:01 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_pipe(char *str, char *cpt)
{
	int i;
	int pipefound;

	pipefound = 0;
	i = 0;
	if (cpt[0] % 2 || cpt[1] % 2)
		return ;
	while (str[i])
	{
		if (str[i] == '|')
			++pipefound;
		else if (!ft_isspace(str[i]))
			pipefound = 0;
		i++;
	}
	if (pipefound >= 2)
		ft_putstr_fd("ERROR TOO MANY PIPES\n", 2);
	cpt[2] = pipefound;
}

char	*add_filler(char **fill)
{
	int		i;
	char	*out;

	out = 0;
	i = 0;
	if (fill && fill[i])
	{
		if ((out = ft_strdup(fill[i])) == NULL)
		{
			freechar2ptr(fill);
			return (0);
		}
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

char	*check_finished_lst(char *line)
{
	char	cpt[3];
	char	**fill;
	char	*out;

	fill = 0;
	out = 0;
	ft_bzero(cpt, 3);
	testfun(line);
	parse_qts(line, cpt);
	// testfun2(line);
	check_pipe(line, cpt);
	if (cpt[0] % 2 || cpt[1] % 2 || cpt[2])
		if ((fill = getfiller(0, cpt)) == 0)
			return (0);
	if ((out = add_filler(fill)) == 0)
		return (0);
	freechar2ptr(fill);
	return (out);
}

void	rdirout(t_list *curr, int mode)
{
	int fd;

	if (curr->next == 0 || curr->next->type != 1)
		return ;
	if (g_mshell.rdirout == 1)
	{
		g_mshell.rdirout = 0;
		close(1);
	}
	if (mode == 0)
		fd = open(curr->next->content, O_APPEND | O_TRUNC | O_WRONLY | O_CREAT
			, 0644);
	else
		fd = open(curr->next->content, O_APPEND | O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("Oh no no no.. FD ERROR\n", 2);
		return ;
	}
	dup2(fd, 1);
	g_mshell.rdirout = 1;
}

void	check_rdir(t_list *curr)
{
	int		fd;

	while (curr && curr->type != 3)
	{
		if (curr->type == 2 || curr->type == 4)
			rdirout(curr, (curr->type == 4));
		else if (curr->type == 5 && curr->next && curr->next->type == 1)
		{
			if (g_mshell.rdirin == 1)
				close(0);
			g_mshell.rdirin = 1;
			fd = open(curr->next->content, O_RDONLY);
			if (fd == -1)
				ft_putstr_fd("Oh no no no.. FD ERROR\n", 2);
			else
				dup2(fd, 0);
		}
		else if (curr->type == 6 && curr->next && curr->next->type == 1)
		{
			open_pipe_n(curr);
			break ;
		}
		curr = curr->next;
	}
}

int		parse_esc(char *str, int j)
{
	int		i;
	int		r;
	int		tra;

	r = 0;
	tra = 0;
	printf("STR B4 = %s\n",str );
	if (*(str + 1) == '\\')
	{
		while (*(str + r) == '\\')
			r++;
		if (*(str + r) == '"' || *(str + r) == '\'')
			r++;
		tra = (r % 2 == 0) ? r - r / 2 : r - (r + 1) / 2;
		r = -tra;
	}
	else if ((*(str + 1) == '\'' || *(str + 1) == '\"') && (tra = 1))
		*str = (*(str + 1) == '\'') ? '\'' : '\"';
	while (++j < tra)
	{
		i = -1;
		while (str[++i])
			str[i] = str[i + 1];
	}
	printf("STR AF = %s\n",str );
	return (r);
}
