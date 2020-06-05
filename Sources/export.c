/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 19:01:40 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/05 19:05:22 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_match(char *env, char *param)
{
	int ret;
	int i ;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	ret = ft_strncmp(env, param, i);
	return (ret);
}

int		unset_var(char *target)
{
	int		i;
	t_list	*curr;

	curr = g_mshell.ls->next;

	i = 0;

	if (g_mshell.vars != NULL)
		while (g_mshell.vars[i])
		{
			if(!check_match(g_mshell.vars[i], target))
			{
				free(g_mshell.vars[i]);
				while (g_mshell.vars[++i])
					g_mshell.vars[i - 1] = g_mshell.vars[i];
				g_mshell.vars[i - 1] = 0;
				return (0);
			}
			i++;
		}
	return (0);
}

void	exportlst(char **envi)
{
	int		x;
	int		y;
	char	**env2;

	x = 0;
	while (envi[x])
		++x;
	if ((env2 = (char **)malloc(sizeof(char *) * (x + 1))) == NULL)
	{
		ft_putstr(strerror(errno));
		return ;
	}
	y = -1;
	while (envi[++y])
		env2[y] = ft_strdup(envi[y]); // PAS PROTEGE
	env2[y] = NULL;
	quicks(env2, 0, y - 1);
	x = -1;
	while (env2[++x])
	{
		y = 0;
		ft_putstr("declare -x ");
		while (env2[x][y] && env2[x][y] != '=' )
			++y;
		write(1, env2[x], y);
		if (env2[x][y] == '=' && env2[x][y + 1] == 0)
			write(1, "=\"\"", 3);
		else if (env2[x][y] != 0 && env2[x][y + 1] != 0)
		{
			write(1, "=\"", 2);
			ft_putstr(env2[x] + y + 1);
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
	}
	freechar2ptr(env2);
}

int		export(char *param)
{
	int		i;
	char	**n_envi;
	t_list	*curr;

	curr = g_mshell.ls->next;
	if (param != NULL)
	{
		printf("Param input = %s\n",param );
		curr = ft_lstnew(param);
	}
	i = 0;
	if (check_valid_export(curr))
	{
		printf("EXPORT NOT VALID\n");
		return (0);
	}
	if (curr == 0)
	{
		exportlst(g_mshell.env); // PAS PROTEGE
		return (0);
	}

	char *tmp;
	if (ft_strchr(curr->content, '=') == NULL)
	{
		tmp = vars(curr->content);
		if (tmp)
		{
			curr->content = ft_strjoinf1(curr->content, "=");
			curr->content = ft_strjoinf1(curr->content, tmp);
		}
	}
	unset_var(curr->content);
	while (g_mshell.env[i])
	{
		if(!check_match(g_mshell.env[i], curr->content))
		{
			free(g_mshell.env[i]);
			g_mshell.env[i] = ft_strdup(curr->content); // PAS PROTEGE
			if (param != NULL)
				free(curr);
			return (0);
		}
		i++;
	}
	if(!(n_envi = malloc(sizeof(char *) * (i + 2))))
		return (1);
	i = -1;
	while (g_mshell.env[++i])
		n_envi[i] = g_mshell.env[i];
	n_envi[i] = ft_strdup(curr->content); // PAS PROTEGE
	n_envi[++i] = 0;
	free(g_mshell.env);
	g_mshell.env = n_envi;
	return (0);
}