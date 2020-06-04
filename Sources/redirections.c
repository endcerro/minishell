/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/04 11:49:07 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/04 18:59:59 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void 	open_pipe_n(t_list *curr)
{
	int *pipes;

	pipes = g_mshell.pipes;
	if (pipes[2] != -1)
		pipes += sizeof(int) * 3;
	pipe(pipes);					//Creating pipe
	pipes[2] = 1;			//Set pipe flag to open
	dup2(pipes[1], 1);		//Set outpout to pipe1
	g_mshell.rdirout = 2;				//Outpout has been redirected
	curr->type = 3; 					//Set | as ;
}

void 	close_pipe_n()
{
	int *pipes;

	pipes = g_mshell.pipes;
	if (g_mshell.rdirin == 2)			//If redir on input
	{
		dup2(g_mshell.oldfdin, 0);		//Reset input
		if (pipes[2] != 0)
			pipes += sizeof(int) * 3;	
		close(pipes[0]);	//Close it
		pipes[2] = -1;		//Set as closed
		
		g_mshell.rdirin = 0;			//Mark rdir input as off
	}
	pipes = g_mshell.pipes;
	if (g_mshell.rdirout == 2)			//If rdir on output
	{
		if (pipes[2] != 1)
			pipes += sizeof(int) * 3;
		dup2(pipes[0], 0);	//Set input to pipe1
		dup2(g_mshell.oldfdout, 1); //Reset output
		close(pipes[1]);	//Close input of pipe
		pipes[2] = 0;		//Pipe input is closed
		g_mshell.rdirout = 0; 			//No output redirs
		g_mshell.rdirin = 2;			//Input redir from pîpe	
	}
}

// void 	open_pipe(t_list *curr)
// {

// 	int *pipe1 = g_mshell.pipes;
// 	int *pipe2 = g_mshell.pipes + sizeof(int) * 3;

// 	if(pipe1[2] == -1) 		//Pipe1 isn't open at all
// 	{
	
// 		pipe(pipe1);			//Creating pipe
// 		pipe1[2] = 1;			//Set pipe flag to open
// 		dup2(pipe1[1], 1);		//Set outpout to pipe1
// 	}
// 	else								//Pipe1 is already open
// 	{
// 		// ft_putstr_fd("Using second pipe\n",2);
									
// 		pipe(pipe2);			//Creating pipe2
// 		pipe2[2] = 1;			//Set pipe flag to open
// 		dup2(pipe2[1], 1);		//Set output to pipe2
// 	}
// 	g_mshell.rdirout = 2;				//Outpout has been redirected
// 	curr->type = 3; 					//Set | as ;
// }

// void 	close_pipe()
// {

// 	int *pipe1 = g_mshell.pipes;
// 	int *pipe2 = g_mshell.pipes + sizeof(int) * 3;

// 	// pipe1 = g_mshell.pipe1;
// 	// pipe2 = g_mshell.pipe2;
// 	// g_mshell.pipe1 = g_mshell .pipes;
// 	// g_mshell.pipe2 = g_mshell.pipes + sizeof(int) * 3;
// 	if(g_mshell.rdirin == 2)			//If redir on input
// 	{
// 		dup2(g_mshell.oldfdin, 0);		//Reset input
// 		if(pipe1[2] == 0)		//If from pipe1
// 		{
// 			close(pipe1[0]);	//Close it
// 			pipe1[2] = -1;		//Set as closed
// 		}
// 		else							//If from pipe2
// 		{								
// 			close(pipe2[0]);	//Close it
// 			pipe2[2] = -1; 	//Set as closed
// 		}
// 		g_mshell.rdirin = 0;			//Mark rdir input as off
// 		// ft_putstr_fd("rdirin = 0\n",2);
// 	}


// 	if (g_mshell.rdirout == 2)			//If rdir on output
// 	{
// 		if(pipe1[2] == 1)		//Case pipe1
// 		{
// 			dup2(pipe1[0], 0);	//Set input to pipe1
// 			dup2(g_mshell.oldfdout, 1); //Reset output
// 			close(pipe1[1]);	//Close input of pipe
// 			pipe1[2] = 0;		//Pipe input is closed
// 		}
// 		else
// 		{
// 			dup2(pipe2[0], 0);	//Set input to pipe2
// 			dup2(g_mshell.oldfdout, 1);	//Reset output
// 			close(pipe2[1]);	//Close input of pipe
// 			pipe2[2] = 0;		//Pipe input is closed
// 		}	
// 		g_mshell.rdirout = 0; 			//No output redirs
// 		g_mshell.rdirin = 2;			//Input redir from pîpe
// 		// ft_putstr_fd("rdirout = 0\n",2);
// 		// ft_putstr_fd("rdirin = 2\n",2);
// 	}
// }