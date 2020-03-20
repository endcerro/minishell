/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/20 23:29:47 by edal--ce          #+#    #+#             */
/*   Updated: 2020/03/21 00:56:38 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_path	*ft_lstnew(char *str)
{
	t_path *out;

	if (!(out = malloc(sizeof(t_path))))
		return (0);
	out->str = str;
	out->next = 0;
	return (out);
}

void	ft_lstadd_back(t_path **alst, t_path *new)
{
	t_path *tmp;

	if (alst && new)
	{
		if (*alst == NULL)
			*alst = new;
		else
		{
			tmp = *alst;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
	}
}

t_path **init_path(char *str)
{
	t_path 	**path;
	int 	i;

	if(!(path = (t_path**)malloc(sizeof(t_path*))))
		return (NULL);
	*path = NULL;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '/' && str[i + 1] != 0)
		{
			ft_lstadd_back(path, ft_lstnew(str + i + 1));
			str[i] = '\0';
		}
	}
	return (path);
}

void free_path(t_path **path)
{
	t_path *tmp;
	t_path *curr;

	curr = *path;
	while(curr)
	{
		tmp = curr;
		curr= curr->next;
		free(tmp);
	}
	free(path);
}

char *join_pwd(t_path **path, char *pwd)
{
	char 	*out;
	t_path 	*curr;

	curr = *path;
	out = ft_strdup("PWD=");
	if(!curr)
		out = ft_strjoinf1(out, "/");
	while(curr)
	{
		out = ft_strjoinf1(out, "/");
		out = ft_strjoinf1(out, curr->str);
		curr = curr->next;
	}
	free(pwd);
	free_path(path);
	return (out);
}
