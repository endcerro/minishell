/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/04 11:49:07 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/18 18:44:16 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		open_pipe_n(t_list *curr)
{
/* 	int		*pipes; */

/* 	pipes = g_mshell.pipes; */
/* 	if (pipes[2] != -1) */
/* 		pipes += sizeof(int) * 3; */
/* 	if (pipe(pipes) == -1) */
/* 		return (ft_printh(2, 1, "%s", strerror(errno))); */
/* 	pipes[2] = 1; */
/* 	if (g_mshell.rdirout == 1) */
/* 	{ */
/* 		if (close(pipes[1]) == -1) */
/* 			ft_putstr_fd("ERROR CLOSING FD", 2); */
/* 		pipes[2] = 0; */
/* 	} */
/* 	else */
/* 	{ */
/* 		if (dup2(pipes[1], 1) == -1) */
/* 			return (ft_printh(2, 1, "%s", strerror(errno))); */
/* 		g_mshell.rdirout = 2; */
/* 	} */
/* 	curr->type = 3; */
	return (0);
}

void	close_pipe_rdro(void)
{
/* 	int *pipes; */

/* 	pipes = g_mshell.pipes; */
/* 	if (g_mshell.rdirout == 2) */
/* 	{ */
/* 		if (pipes[2] != 1) */
/* 			pipes += sizeof(int) * 3; */
/* 		if (dup2(pipes[0], 0) == -1) */
/* 		{ */
/* 			ft_putendl(strerror(errno)); */
/* 			exit(0); */
/* 		} */
/* 		if (dup2(g_mshell.oldfdout, 1) == -1) */
/* 		{ */
/* 			ft_putendl(strerror(errno)); */
/* 			exit(0); */
/* 		} */
/* 		if (close(pipes[1]) == -1) */
/* 			ft_putstr_fd("ERROR CLOSING FD", 2); */
/* 		pipes[2] = 0; */
/* 		g_mshell.rdirout = 0; */
/* 		g_mshell.rdirin = 2; */
/* 	} */
	;
}

void	close_pipe_n(void)
{
/* 	int		*pipes; */

/* 	pipes = g_mshell.pipes; */
/* 	if (g_mshell.rdirin == 2) */
/* 	{ */
/* 		if (dup2(g_mshell.oldfdin, 0) == -1) */
/* 		{ */
/* 			ft_putendl(strerror(errno)); */
/* 			exit(0); */
/* 		} */
/* 		if (pipes[2] != 0) */
/* 			pipes += sizeof(int) * 3; */
/* 		if (close(pipes[0]) == -1) */
/* 			ft_putstr_fd("ERROR CLOSING FD", 2); */
/* 		pipes[2] = -1; */
/* 		g_mshell.rdirin = 0; */
/* 	} */
/* 	close_pipe_rdro(); */
	;
}
