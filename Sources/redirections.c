/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/04 11:49:07 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/05 14:52:27 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_pipe_n(t_list *curr)
{
	int		*pipes;

	pipes = g_mshell.pipes;

	if (pipes[2] != -1)
		pipes += sizeof(int) * 3;
	pipe(pipes);
	pipes[2] = 1;

	if(g_mshell.rdirout == 1)
	{
		close(pipes[1]); 		//Closing the read end of pipe
		pipes[2] = 0;			//Setting it as closed 
	}
	else
	{
		dup2(pipes[1], 1);
		g_mshell.rdirout = 2;		
	}
	curr->type = 3;
}

void	close_pipe_n(void)
{
	int		*pipes;

	pipes = g_mshell.pipes;

	if (g_mshell.rdirin == 2)
	{
		// ft_putstr_fd("Here2",2);
		dup2(g_mshell.oldfdin, 0);
		if (pipes[2] != 0)
			pipes += sizeof(int) * 3;
		close(pipes[0]);
		pipes[2] = -1;
		g_mshell.rdirin = 0;
	}
	pipes = g_mshell.pipes;
	if (g_mshell.rdirout == 2)
	{
		if (pipes[2] != 1)
			pipes += sizeof(int) * 3;
		dup2(pipes[0], 0);
		dup2(g_mshell.oldfdout, 1);
		close(pipes[1]);
		pipes[2] = 0;
		g_mshell.rdirout = 0;
		g_mshell.rdirin = 2;
	}
}
