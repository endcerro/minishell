/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/18 18:34:51 by edal--ce          #+#    #+#             */
/*   Updated: 2020/04/28 16:23:39 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <limits.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>

#include <stdio.h>

extern char		**environ;

typedef struct 	s_mshell
{
	char		**env;
	char		**vars;
	char		**params;
	pid_t		pid;
}				t_mshell;

t_mshell		mshell;

char 			**get_blocks(char *line);
char 			*env(char **envi, char **params, char *test);
void 			parse_env(char **param, char **envi);
void			commandorvar(char ***envi, char **params, char ***vars);

#endif
