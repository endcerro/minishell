/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/18 18:34:51 by edal--ce          #+#    #+#             */
/*   Updated: 2020/03/21 00:56:10 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <limits.h>

#include <stdio.h>

extern char **environ;

typedef struct 	s_path
{
	char 			*str;
	struct s_path 	*next;
}				t_path;

char 			**get_blocks(char *line);
char			**commandorvar(char ***envi, char **params, char ***vars);
/* t_path 			**init_path(char *str); */
/* void 			free_path(t_path **path); */
/* char 			*join_pwd(t_path **path, char *pwd); */




#endif
