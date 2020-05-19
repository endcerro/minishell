/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 19:21:47 by edal--ce          #+#    #+#             */
/*   Updated: 2020/05/19 10:24:45 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_valid_export(char **params)
{
	int i;

	i = 0;
	if (params[1])
	{
		while (params[1][i])
			if (ft_isspace(params[1][i++]))
				return (0);
		if (ft_strchr(params[1], '='))
			return (1);
	}
	if (params[2] && params[2][0] == '=')
	{
		params[1] = ft_strjoinft(params[1], params[2]);
		i = 2;
		while (params[++i])
		{
			params[i - 1] = params[i];
		}
		params[i - 1] = 0;
		return (1);
	}
	return (0);
}

void check_command(int i )
{
	char **copy = g_mshell.params;

	int newfd = 0;
	int		cpt[2];

	ft_bzero(cpt, 2);


	char *finish = 0;

	finish = check_finished1();
	
	int j = 0;
	while(finish && g_mshell.params[j])
	{
		printf("finishing..\n");
		j++;
	}
	if(finish)
	{
		printf("blocks = %d\n",count_blocks(finish));
		g_mshell.params[j - 1] = ft_strjoinft(g_mshell.params[j - 1], finish);
	}

	// // printf("Parmas listing ;\n");
	while(g_mshell.params[i])
	{
		// g_mshell.params[i] = parse_qts(g_mshell.params[i], cpt);

		// if (cpt[0] % 2 || cpt[1] % 2)
		// 	out = getfiller(0, cpt);

		if(g_mshell.params[i][0] == '>')
		{
			if (g_mshell.params[i + 1])
			{
				newfd = open(g_mshell.params[i + 1],O_WRONLY | O_APPEND | O_CREAT, 0644);
				
			}
			printf("REDIRECTION\n");
		}
		else if(g_mshell.params[i][0] == ';')
		{
			printf("OTHER COMMAND\n");
			g_mshell.params[i] = 0;
			g_mshell.params = g_mshell.params + i + 1;
			// printf("next param to evaluate = %s\n",g_mshell.params[0] );
			check_command(0);
			g_mshell.params = copy;
			// g_mshell.params[i] = 0;
		}
		// else
			i++;
			// printf("%s\n", g_mshell.params[i++]);		
		
	}
	g_mshell.params = copy;
	// 	// printf("\neol\n\n");
	// i = 0;
	int oldfd = dup(1);
	if(newfd > 0)
	{
		dup2(newfd, 1);
	}
	
	checkinput();
	dup2(oldfd,1);



	return ;
}