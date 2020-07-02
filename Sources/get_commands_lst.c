/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:45 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/02 15:19:37 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		expand_vars(t_list *lst)			//PROTECTED AND LEAK FREE
{
	t_list *curr;
	t_list *prev;

	curr = lst;
	prev = 0;
	while (curr && curr->type != 3)
	{
		if (curr->content[0] != '\'')
		{
			parse_env_ls(&(curr->content));
			if (curr->content[0] == 0)
			{
				if (prev)
				{
					prev->next = curr->next;
					ft_lstdelone(curr);
				}
			curr = prev;
			}

		}
		if(curr->content == 0)
		{
			return (1);
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}

int		isstrdigit(char *str)
{
	while (*str && (*str == '-' || *str == '+'))
		++str;
	while (*str)
	{
		if (ft_isdigit(*str) == 0 )
			return (0);
		++str;
	}
	return (1);
}

void	deconechar(char *str)
{
	int i;

	i = 0;
	if (*str == 0)
		return ;
	while (str[++i])
	{
		str[i - 1] = str[i];
	}
	str[i - 1] = str[i];
}

void	trimbs(t_list *curr)
{
	int i;
	int j;
	int qtcp;

	while (curr && curr->content && curr->type == 1)
	{
		// printf("IN trimbs %s\n",curr->content );
		i = 0;
		if (curr->content[0] != '\'')
			while (curr->content[i])
			{
				if (curr->content[i] == '\\')
				{
					// printf("\\ found\n");
					qtcp = 0;
					j = i;
					while (curr->content[j] == '\\')
					{
						qtcp++;
						j++;
					}

					// qtcp = (qtcp % 2 == 0) ? qtcp : qtcp - 1;

					if ((qtcp != 1 && qtcp % 2 == 0) || curr->content[0] == '\"')
					{
						qtcp /= 2;
					}
					else if (qtcp != 1)
					{
						qtcp--;
						qtcp /= 2;
						qtcp++;
					}

					if (curr->content[0] == '\"' && curr->content[j] < 0)
						qtcp++;
					// printf("qtcp = %d\n",qtcp );
					for (int z = 0; z < qtcp; z++)
					{
						// printf("Trimming one char qt pos %d \n", i);
						deconechar(curr->content + i);
					}

					i += qtcp;
				}
				else
					i++;
			}
		curr = curr->next;
	}
}


int check_valid(t_list *lst)
{
	int cp_r;
	int cp_d;

	cp_r = 0;
	cp_d = 0;

	while (lst)
	{
		if (lst->type == 3 || lst->type == 6)
		{
			cp_d++;
		}
		else if (lst->type != 1)
			cp_r++;
		else
		{
			cp_r = 0;
			cp_d = 0;
		}
		if (cp_d > 1 || cp_r > 1)
		{
			ft_printh(2,1, "minishell: syntax error near unexpected token \'%s\'\n", lst->content);		
			return (0);
		}
		lst = lst->next;
	}
	return 1;
}

void	checkinput_ls(char *line)
{
	t_list	*curr;
	t_list	*copy;
	int		ex;

	curr = g_mshell.ls;
	if (curr == 0)
		return ;
	escape_lst(curr);
	if (g_mshell.ls->type != 1)
	{
		ft_printh(2,1, "minishell: syntax error near unexpected token \'%s\'\n", g_mshell.ls->content);
		return ;
	}
	if (check_valid(curr) == 0)
	{
		return ;
	}


	if(expand_vars(curr))
	{
		return ;
	}

	trimbs(curr);

	correctlst(curr);
	mergelst(curr);


	if (check_rdir(curr) == 1)
		return;	
	ft_lstprint(curr);

	if (ft_strcmp(g_mshell.ls->content, "exit") == 0)
	{
		ex = 0;
		ft_putstr("exit\n");
		if (g_mshell.ls->next && g_mshell.ls->next->type == 1)
		{
			if (isstrdigit(g_mshell.ls->next->content) == 0)
				ft_printh(2, 0, "minishell: exit: %s: numeric argument needed\n", g_mshell.ls->next->content);
			else if (g_mshell.ls->next->next && g_mshell.ls->next->next->type == 1)
			{
				ft_putstr_fd("minishell: exit: too many arguments\n", 2);
				return ;
			}
			else
				ex = ft_atoi(g_mshell.ls->next->content);
		}
		freechar2ptr(g_mshell.env);
		freechar2ptr(g_mshell.vars);
		ft_lstclear(&g_mshell.ls);
		free(line);
		exit(ex);
	}
	else if (ft_strcmp(g_mshell.ls->content, "echo") == 0)
		g_mshell.exitcode = echo_ls();						//PROTECTED
	else if (ft_strcmp(g_mshell.ls->content, "env") == 0) //PROTECTED UNTILL HERE
	{
		env(NULL);
		g_mshell.exitcode = 0;
	}
	else if (ft_strcmp(g_mshell.ls->content, "cd") == 0)
		g_mshell.exitcode = cd();							//CHECK PROTECTION
	else if (ft_strcmp(g_mshell.ls->content, "pwd") == 0)
		g_mshell.exitcode = pwd();
	else if (ft_strcmp(g_mshell.ls->content, "export") == 0)
		g_mshell.exitcode = export(0);							//PROTECTED
	else if (ft_strcmp(g_mshell.ls->content, "unset") == 0)
		g_mshell.exitcode = unset();							//PROTECTED
	else if (ft_strcmp(g_mshell.ls->content, "clear") == 0)
		ft_putstr("\033c");
	else
		commandorvar();
	copy = g_mshell.ls;
	int test = 0;

	if (g_mshell.rdirout == 1)
	{
		if (close(dup(1)) == -1)
			ft_putstr_fd("ERROR CLOSING FD", 2);
		dup2(g_mshell.oldfdout, 1);
		g_mshell.rdirout = 0;


		if(g_mshell.pipes[2] == 0)
		{
			if(g_mshell.rdirin != 2)
			{
				test = 1;
				dup2(g_mshell.pipes[0], 0);
				g_mshell.rdirin = 2;
			}

		}
	}
	if (g_mshell.rdirin == 1)
	{
		//ft_putstr_fd("CLOSING FD\n", 2);
		if (close(dup(0)) == -1)
			ft_putstr_fd("ERROR CLOSING FD\n", 2);
		// close(dup(0));
		// ft_putstr_fd("0 CLOSED\n", 2);
		dup2(g_mshell.oldfdin, 0);
		g_mshell.rdirin = 0;
	}
	if (test == 0)
		close_pipe_n();
	// if(test == 1 &&  )
	while (curr)
	{
		if (curr->type == 3 && curr->next != NULL)
		{
			g_mshell.ls = curr->next;
			checkinput_ls(line);
			break ;
		}
		curr = curr->next;
	}
	g_mshell.ls = copy;
}
