/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/04 11:49:07 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/04 18:02:01 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void 	open_pipe(t_list *curr)
{
	if(g_mshell.pipe1[2] == -1) 		//Pipe1 isn't open at all
	{
		// ft_putstr_fd("Using first pipe\n",2);
		pipe(g_mshell.pipe1);			//Creating pipe
		g_mshell.pipe1[2] = 1;			//Set pipe flag to open
		dup2(g_mshell.pipe1[1], 1);		//Set outpout to pipe1
	}
	else								//Pipe1 is already open
	{
		// ft_putstr_fd("Using second pipe\n",2);
									
		pipe(g_mshell.pipe2);			//Creating pipe2
		g_mshell.pipe2[2] = 1;			//Set pipe flag to open
		dup2(g_mshell.pipe2[1], 1);		//Set output to pipe2
	}
	g_mshell.rdirout = 2;				//Outpout has been redirected
	curr->type = 3; 					//Set | as ;
}

void 	close_pipe()
{
	if(g_mshell.rdirin == 2)			//If redir on input
	{
		dup2(g_mshell.oldfdin, 0);		//Reset input
		if(g_mshell.pipe1[2] == 0)		//If from pipe1
		{
			close(g_mshell.pipe1[0]);	//Close it
			g_mshell.pipe1[2] = -1;		//Set as closed
		}
		else							//If from pipe2
		{								
			close(g_mshell.pipe2[0]);	//Close it
			g_mshell.pipe2[2] = -1; 	//Set as closed
		}
		g_mshell.rdirin = 0;			//Mark rdir input as off
		// ft_putstr_fd("rdirin = 0\n",2);
	}

	if (g_mshell.rdirout == 2)			//If rdir on output
	{
		if(g_mshell.pipe1[2] == 1)		//Case pipe1
		{
			dup2(g_mshell.pipe1[0], 0);	//Set input to pipe1
			dup2(g_mshell.oldfdout, 1); //Reset output
			close(g_mshell.pipe1[1]);	//Close input of pipe
			g_mshell.pipe1[2] = 0;		//Pipe input is closed
		}
		else
		{
			dup2(g_mshell.pipe2[0], 0);	//Set input to pipe2
			dup2(g_mshell.oldfdout, 1);	//Reset output
			close(g_mshell.pipe2[1]);	//Close input of pipe
			g_mshell.pipe2[2] = 0;		//Pipe input is closed
		}	
		g_mshell.rdirout = 0; 			//No output redirs
		g_mshell.rdirin = 2;			//Input redir from pîpe
		// ft_putstr_fd("rdirout = 0\n",2);
		// ft_putstr_fd("rdirin = 2\n",2);
	}
}