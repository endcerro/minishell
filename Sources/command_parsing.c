/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:49 by edal--ce          #+#    #+#             */
/*   Updated: 2020/05/28 15:13:41 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	*check_finished_lst(char *line)	//Chnager le char ** en char * ?
{
	int		i;
	int		cpt[2];
	char	**fill;
	char 	*out;

	i = -1;
	fill = 0;
	ft_bzero(cpt, 2);
	cpt[0] = 0;
	cpt[1] = 0;
	parse_qts(line, cpt);

	if (cpt[0] % 2 || cpt[1] % 2)
	{
	 	fill = getfiller(0, cpt); // PAS PROTEGE+
	}
	out = 0;
	i = 0;
	if(fill && fill[i])
	{
		out = ft_strdup(fill[i++]);
		while(fill[i])
		{
			out = ft_strjoinf1(out, "\n");
			out = ft_strjoinf1(out, fill[i++]);
		}
	}
	return (out);
}

void add_file_ls(t_list *ls)
{
	// t_list *curr;
	// t_list *new;
	// int fd;
	// char *out;
	// char *buff;

	// // printf("current = %s \n",ls->content);
	// curr = ls;

	// out = ft_strdup("");
	// if(fd == -1)
	// {
	// 	printf("ERROR OPENING FILE : %s\n",curr->next->content);
	// }
	// while(get_next_line(fd, &buff))
	// {
	// 	out = ft_strjoinft(out, buff);
	// 	out = ft_strjoinf1(out, "\n");
	// }

	// curr->content[0] = ' ';
	// curr->content[1] = 0;
	// curr->next->content = out;
	// free(curr->content);
	// curr->content = out;

	// new = curr->next->next; 
	// ft_lstdelone(curr->next);
	// curr->next = new;
	// curr->type = 1;
	// free(buff);
	// printf("done reading :\n%s", out);
}

void check_rdir()
{
	t_list *curr;
	int fd;

	curr = g_mshell.ls;
	


	while(curr && curr->type != 3)
	{
		if (curr->type == 2 && curr->next && curr->next->type == 1)
		{
			g_mshell.oldfd = dup(1);
			fd = open(curr->next->content,
			 O_APPEND | O_TRUNC | O_WRONLY | O_CREAT, 0644);
			dup2(fd, 1);
		}
		else if (curr->type == 4 && curr->next && curr->next->type == 1)
		{
			g_mshell.oldfd = dup(1);
			fd = open(curr->next->content,
			 O_APPEND | O_WRONLY | O_CREAT, 0644);
			dup2(fd, 1);	
		}
		else if (curr->type == 5 && curr->next && curr->next->type == 1)
		{
			g_mshell.oldfd_in = dup(0);
			fd = open(curr->next->content, O_RDONLY);
			dup2(fd, 0);
			// add_file_ls(curr);
		}
		curr = curr->next;
	}
}

int		parse_esc(char *str)
{
	int i;
	int j;
	int r;
	int tra;

	r = 0;
	tra = 0;
	j = -1;
	if (*(str + 1) == '\\')
	{
		while (*(str + r) == '\\')
			r++;
		if (*(str + r) == '"' || *(str + r) == '\'')
			r++;
		tra = (r % 2 == 0) ? r - r / 2 : r - (r + 1) / 2;
		r = -tra;
	}
	else if ((*(str + 1) == '\'' || *(str + 1) == '\"') && (tra = 1))
		*str = (*(str + 1) == '\'') ? '\'' : '\"';
	while (++j < tra)
	{
		i = -1;
		while (str[++i])
			str[i] = str[i + 1];
	}
	return (r);
}