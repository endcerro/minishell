/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/18 18:34:51 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/05 15:24:22 by edal--ce         ###   ########.fr       */
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
	int 			type;
}					t_list;

typedef struct 	s_mshell
{
	char 		*line;
	char		**env;
	char		**vars;
	t_list		*ls;
	pid_t		pid;
	int			exitcode;

	char 		rdirin;
	char 		rdirout;

	int 		pipe1[3];
	int 		pipe2[3];
	int 		pipes[6];

	char 		pipnb;

	int 		oldfdout;
	int 		oldfdin;


}				t_mshell;

t_mshell		g_mshell;

int 			get_blocks(char *line);
int				export(char *params);
char 			*env(char *request);
char 			*vars(char *request);
void 			parse_env_ls(char **param);
void			commandorvar(void);
void			freechar2ptr(char **ptr);


int 			check_valid_export(char **params);
void 			parse_qts(char *str, int *cpt);
int				parse_bs(char *str);
int				parse_esc(char *str, int i);
char			**getfiller(int depth, int *cpt);
void			freechar2ptr(char **ptr);

void 			checkinput_ls(void);
void 			check_pipe(char *str, int *cpt);
char			*get_lst(char *line);
char 			*inside_join(char *base, char *add);
char 			*env(char *request);
int				pwd();
int				cd();
int				unset();

void			ft_lstclear(t_list **lst);
t_list			*ft_lstnew(char *content);
void 			check_rdir();
void			ft_lstadd_back(t_list **alst, t_list *new);
char			*check_finished_lst(char *line);
void			ft_lstadd_next(t_list *alst, t_list *new);
void			ft_lstdelone(t_list *lst);

char			*ft_itoaa(int n);


void 			close_pipe_n();
void 			open_pipe_n(t_list *curr);


void			ft_lstprint(t_list *lst);


#endif
