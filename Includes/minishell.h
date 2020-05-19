/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/18 18:34:51 by edal--ce          #+#    #+#             */
/*   Updated: 2020/05/19 12:41:34 by edal--ce         ###   ########.fr       */
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

typedef struct		s_list
{
	char			*content;
	struct s_list	*next;
}					t_list;

typedef struct 	s_mshell
{
	char 		*line;
	char		**env;
	char		**vars;
	char		**params;
	t_list		*ls;
	pid_t		pid;
	int			exitcode;
}				t_mshell;

t_mshell		g_mshell;

int 			get_blocks(char *line);
void			export(char **params);
char 			*env(char *request);
char 			*vars(char *request);
void 			parse_env(char **param);
void			commandorvar(void);
void			freechar2ptr(char **ptr);

char			**check_finished();
int 			check_valid_export(char **params);
void 			parse_qts(char *str, int *cpt);
int				parse_bs(char *str);
int				parse_esc(char *str);
char			**getfiller(int depth, int *cpt);
void			check_command(int i );
void	checkinput(void);
char	*check_finished1();	//Chnager le char ** en char * ?
void	freechar2ptr(char **ptr);
int		count_blocks(char *line);

void	get_lst(char *line);
char 	*inside_join(char *base, char *add);







#endif
