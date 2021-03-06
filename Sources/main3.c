/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/11 21:39:02 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/09 21:11:04 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		checklstate(char **oline, char **line)
{
	if (g_mshell.linestate == 1)
	{
		*oline = ft_strjoinft(*oline, *line);
		ft_putstr("  \b\b");
		return (1);
	}
	else if (**line)
	{
		g_mshell.linestate = 1;
		*oline = *line;
		ft_putstr("  \b\b");
		return (1);
	}
	return (0);
}

void	dealwithlstate2(char **oline)
{
	if (g_mshell.linestate == 2)
	{
		g_mshell.linestate = 0;
		free(*oline);
		*oline = NULL;
	}
}

void	mainloop(int ret, char *oline, char **line)
{
	while (1 | (ret = prompt(line)))
	{
		if (*line == NULL || ret < 0)
			return ;
		dealwithlstate2(&oline);
		if (ret == 0)
		{
			if (checklstate(&oline, line) == 1)
				continue ;
			else
				return ;
		}
		else if (oline && ((*line = ft_strjoinft(oline, *line)) || 1))
			if ((oline = NULL) || 1)
				g_mshell.linestate = 0;
		if ((*line = get_lst(*line)) == NULL)
			continue ;
		checkinput_ls(*line, g_mshell.ls, g_mshell.ls, 0);
		ft_lstclear(&g_mshell.ls);
		free(*line);
		*line = NULL;
	}
}
