/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/18 18:34:51 by edal--ce          #+#    #+#             */
/*   Updated: 2020/03/20 23:46:22 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <unistd.h>
# include <string.h>
# include <errno.h>

#include <stdio.h>

extern char **environ;

typedef struct 	s_path
{
	char 			*str;
	struct s_path 	*next;
}				t_path;

char 			**get_blocks(char *line);
t_path 			**init_path(char *str);
void 			print_path(t_path *path);
void 			free_path(t_path **path);



#endif
