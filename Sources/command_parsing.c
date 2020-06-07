/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:49 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/06 18:54:22 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void 	check_pipe(char *str, int *cpt)
{
	int i;
	int pipefound;

	pipefound = 0;
	i = 0;

	if (cpt[0] % 2 || cpt[1] % 2)
		return ;
	while(str[i])
	{
		if(str[i] == '|')
			++pipefound;
		else if(!ft_isspace(str[i]))
			pipefound = 0;

		i++;
	}
	if(pipefound >= 2)
		printf("ERROR TOO MANY PIPES\n");
	cpt[2] = pipefound;
}

char	*check_finished_lst(char *line)
{
	int		i;
	int		cpt[3];
	char	**fill;
	char	*out;

	fill = 0;
	i = 0;
	out = 0;
	ft_bzero(cpt, 3);
	cpt[0] = 0;
	cpt[1] = 0;
	cpt[2] = 0;
	parse_qts(line, cpt);
	check_pipe(line, cpt);
	if (cpt[0] % 2 || cpt[1] % 2 || cpt[2])
	{
		fill = getfiller(0, cpt);
		if(fill == 0)
		{
			// free(line);
			return (0);
			//IF PB
		}
	}
	if (fill && fill[i])
	{
		if ((out = ft_strdup(fill[i])) == NULL)
		{
			freechar2ptr(fill);
			return (NULL);
		}
		while (fill[++i])
		{
			if ((out = ft_strjoinf1(out, "\n")) == NULL)
				break ;
			if ((out = ft_strjoinf1(out, fill[i])) == NULL)
				break ;
		}
	}
	freechar2ptr(fill);
	return (out);
}

// int getfd(int type ,int fd)
// {
// 	if(fd)
// }

void	check_rdir(void)
{
	t_list	*curr;
	int		fd;
	int 	flags;

	fd = 0;

	curr = g_mshell.ls;
	while (curr && curr->type != 3)
	{
		if (curr->type == 2 && curr->next && curr->next->type == 1)
		{

			if(g_mshell.rdirout == 1)
				close(1);

			g_mshell.rdirout = 1;
			fd = open(curr->next->content,
				O_APPEND | O_TRUNC | O_WRONLY | O_CREAT, 0644);
			if(fd == -1)
				ft_putstr_fd("Oh no no no.. FD ERROR\n",2);
			else
				dup2(fd, 1);
		}
		else if (curr->type == 4 && curr->next && curr->next->type == 1)
		{
			if(g_mshell.rdirout == 1)
				close(1);
			g_mshell.rdirout = 1;
			fd = open(curr->next->content,
				O_APPEND | O_WRONLY | O_CREAT, 0644);
			if(fd == -1)
				ft_putstr_fd("Oh no no no.. FD ERROR\n",2);
			else
				dup2(fd, 1);
		}
		else if (curr->type == 5 && curr->next && curr->next->type == 1)
		{
			if(g_mshell.rdirin == 1)
				close(0);
			g_mshell.rdirin = 1;
			fd = open(curr->next->content, O_RDONLY);
			if(fd == -1)
				ft_putstr_fd("Oh no no no.. FD ERROR\n",2);
			else
				dup2(fd, 0);
		}
		else if (curr->type == 6 && curr->next && curr->next->type == 1)
		{
			// ft_putstr_fd("Current rdirs : ", 2);
			// ft_putnbr_fd(g_mshell.rdirin, 2);
			// ft_putstr_fd(" ", 2);
			// ft_putnbr_fd(g_mshell.rdirout, 2);
			// ft_putstr_fd("\n", 2);
			open_pipe_n(curr);
			// ft_putstr_fd("Current rdirs : ", 2);
			// ft_putnbr_fd(g_mshell.rdirin, 2);
			// ft_putstr_fd(" ", 2);
			// ft_putnbr_fd(g_mshell.rdirout, 2);
			// ft_putstr_fd("\n", 2);
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
	return (r);
}
