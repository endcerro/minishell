/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/18 18:34:51 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/25 10:23:39 by edal--ce         ###   ########.fr       */
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
# include <signal.h>

#include <stdio.h>

extern char		**environ;

typedef struct		s_list
{
	struct s_list	*next;
	char			*content;
	int 			type;
}					t_list;

typedef struct 	s_mshell
{
	char		**env;
	char		**vars;
	t_list		*ls;

	pid_t		pid;
	int			exitcode;

	int 		pipes[6];
	char 		rdirin;
	char 		rdirout;
	int 		oldfdout;
	int 		oldfdin;

}				t_mshell;

t_mshell		g_mshell;

//BUILTINS
int				pwd();
int				cd();
int				unset();
int 			echo_ls();
int				export(char *params);
char 			*env(char *request);
int				commandorvar(void);
//BUILTIN UTILS
void 			checkinput_ls(char *line);
void 			parse_env_ls(char **param);
char 			*vars(char *request);
int				check_match(char *env, char *param);
char 			*inside_join(char *base, char *add, int mode);
int 			check_valid_export(char **params);
char			*getcwdwrap(void);
//COMMAND PARSING
char			*get_lst(char *line);
int 			get_blocks(char *line);
char			**getfiller(int depth, char *cpt);
void 			parse_qts(char *str, char *cpt);
int				parse_bs(char *str);
int				parse_esc(char *str, int i);
char			*check_finished_lst(char *line);
//IO
void 			check_rdir();
void 			check_pipe(char *str, char *cpt);
void 			close_pipe_n();
void 			open_pipe_n(t_list *curr);
//LST UTILS
t_list			*tag_lst(t_list *lst);
char			*get_lst(char *line);
t_list			*ft_lstnew(char *content);
t_list			*ft_lstnew_p(char *content);
int				ft_lstclear(t_list **lst);
void			ft_lstprint(t_list *lst);
void			ft_lstadd_next(t_list *alst, t_list *new);
void			ft_lstdelone(t_list *lst);
void			ft_lstadd_back(t_list **alst, t_list *new);
//UTILS
int 			freeret(void *ptr, int ret);
void			freechar2ptr(char **ptr);
void			quicks(char **tab, int lo, int hi);
char			*ft_itoaa(int n);
int 			wordlen(char *str);
void 			trim_quotes(char *str);

void 	testfun(char *line);
void 	correctlst(t_list *lst);
void 	testfun2(char *line);

#endif
