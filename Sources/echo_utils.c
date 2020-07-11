/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 16:22:26 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/11 21:53:17 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		parse_env_ls_two(char *query, char **str, char *d_pos)
{
	if (*query == 0 || ft_isspace(*query))
		*(d_pos) = -1;
	else
	{
		if (query[0] != '?' && env(query))
		{
			if (env(query)[0] != 0)
				*str = inside_join(*str, env(query) + 1, 1, 0);
			else
			{
				free(*str);
				*str = ft_strdup("");
			}
		}
		else if (query[0] == '?')
			*str = inside_join(*str, ft_itoaa(g_mshell.exitcode), 3, 1);
		else
			*str = inside_join(*str, vars(query), 1, 0);
		if (*str == 0)
		{
			free(query);
			return (1);
		}
	}
	return (0);
}

void	parse_env_ls(char **str, int len)
{
	char	*d_pos;
	char	*query;

	d_pos = (char *)1;
	while (d_pos != NULL)
	{
		len = 0;
		d_pos = ft_strchr(*str, '$');
		if (d_pos == 0 || d_pos[0] == 0)
			continue ;
		while (ft_isalnum(d_pos[len + 1]))
			len++;
		if (len == 0 && d_pos[len + 1] == '?')
			len++;
		if ((query = ft_substr(d_pos, 1, len)) == 0)
		{
			free(*str);
			*str = 0;
			return ;
		}
		if (parse_env_ls_two(query, str, d_pos))
			return ;
		free(query);
	}
	swap_char(*str, '$');
}

int		parse_bs(char *str)
{
	int		i;

	i = -1;
	while (str[++i])
		if (str[i] == '\\')
			i -= parse_esc(str + i, 0);
	return (0);
}

void	parse_qts(char *str, char *cpt)
{
	int		j;
	int		v;

	j = -1;
	while (str[++j])
	{
		if ((str[j] == '\'' || str[j] == '\"'))
		{
			v = 0;
			if ((v % 2 == 0) && !(cpt[str[j] == '\"'] % 2))
				cpt[!(str[j] == '\"')]++;
		}
	}
}
