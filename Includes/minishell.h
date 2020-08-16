/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/18 18:34:51 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/16 18:59:55 by hpottier         ###   ########.fr       */
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

# define SYNTE "minishell: syntax error near unexpected token \'%s\'\n"
# define SYNTERR2 "minishell: syntax error near unexpected token `%s'\n"
# define SYNTERR3 "minishell: syntax error near unexpected token `newline'\n"
# define NOTVALID "minishell: export: \'%s\': not a valid identifier\n"
# define BSTXT "minishell: unexpected EOF while looking for matching `%s\'\n%s"
# define BSTXT2 "minishell: syntax error: unexpected end of file\n"
# define MERR "minishell: %s\n"
# define FORDIR "minishell: %s: no such file or directory\n"

typedef struct		s_list
{
	struct s_list	*next;
	char			*content;
	int				nospace;
	int				type;
	int				rawtext;
}					t_list;

typedef struct		s_mshell
{
	char		**env;
	char		**vars;
	t_list		*ls;

	int			linestate;
	int			dquote;

	pid_t		pid;
	int			exitcode;
	int			sigswitch;

	int			oldfdout;
	int			oldfdin;

	int			*pipes;
	int			*pidtab;
}					t_mshell;

typedef struct		s_innsplitl
{
	char	**split;
	t_list	*new;
	int		status;
	t_list	*tml;
}					t_innsplitl;

t_mshell			g_mshell;

/*
**BUILTINS
*/

int					pwd();
int					cd(char *pwd, int x, int ret);
int					unset(char *param);
int					echo_ls();
int					export(char *params);
char				*env(char *request);
int					commandorvar(int *npipe, int i);

/*
**BUILTIN UTILS
*/

int					checkinput_ls(char *line, t_list *tmp,
								t_list *copy, int npipe);
int					parse_env_ls(char **param, int len, t_list *curr);
char				*vars(char *request);
int					check_match(char *env, char *param);
char				*inside_join(char *base, char *add, int mode, int ex);
int					check_valid_export(char *str);
char				*getcwdwrap(void);

/*
**COMMAND PARSING
*/

char				*get_lst(char *line);
int					get_blocks(char *line);
char				**getfiller(int depth, char *cpt, int *err);
void				parse_qts(char *str, char *cpt);
int					parse_bs(char *str);
int					parse_esc(char *str, int i);
char				*check_finished_lst(char *line, int *err);

/*
**LST UTILS
*/

t_list				*tag_lst(t_list *lst);
t_list				*ft_lstnew(char *content);
t_list				*ft_lstnew_p(char *content);
int					ft_lstclear(t_list **lst);
void				ft_lstprint(t_list *lst);
void				ft_lstdelone(t_list *lst);
void				ft_lstadd_back(t_list **alst, t_list *new);

/*
**UTILS
*/

int					freeret(void *ptr, int ret);
int					freechar2ptr(char **ptr, int ret);
void				quicks(char **tab, int lo, int hi);
char				*ft_itoaa(int n);
int					wordlen(char *str);
void				trim_quotes(char *str);

t_list				*split_line_lst(char *line, int i);
void				escape_chars(char *line, int i, int j, char *cpt);
int					correctlst(t_list *lst);
void				de_escape_chars(char *line, int i);
void				escape_lst(t_list *lst);
int					mergelst(t_list *curr);
void				sigint(int sig);
void				sigquit(int sig);
char				*checkpath(int j, char **params);
int					addvar(char *str);
char				**ls_params(void);
void				swap_char(char *str, char c);
int					check_valid(t_list *lst, int cp_r, int cp_d, t_list *first);
int					expand_vars(t_list *curr);
void				trimbs(t_list *curr);
int					isstrdigit(char *str);
int					unset_var(char *target);
int					exportlst(char **envi);
char				*get_word_lst(char *line, int *p, size_t len);
t_list				*inner_split(t_list *lst);
int					isquote(char c);
void				find_char(char *buff);
t_list				*inner_split_loop(t_list *curr, char *buff, int i, int j);
void				mainloop(int ret, char *oline, char **line);
void				dealwithlstate2(char **oline);
int					checklstate(char **oline, char **line);
int					prompt(char **line);
void				deconechar(char *str);
char				*rethomedir(void);
int					retunset(char *str);
void				print_export(char **env2);
int					checkspace(char *str);
int					trim_rdir(t_list *lst);
int					islastrdir(t_list *lst, int type);
int					exportstuffter(t_list *curr, char *tmp);
int					expand_t(char **str);
void				exec_command(char *line, t_list *lst,
								int *npipe, int oldfd);
int					countpipes(t_list *curr);
int					allocpipepid(int npipe, t_list *copy);
int					prep_ls(t_list *curr, t_list *lst, int ret);
t_list				*correct_rdir(t_list *lst, t_list *curr,
								t_list *newlst, t_list *pr);
int					openrdir3(t_list **urr, char *file);
int					ms_exit(char *line, int *npipe);
int					check_end_pipe(char *line, char *cpt);
int					baderrorohno(void);

#endif
