/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 16:22:26 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/22 15:04:14 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		parse_env_ls_two(char *query, char **str, char *d_pos)
{
	int		len;
	int		i;

	i = -1;
	if ((*query == 0 || ft_isspace(*query)) && d_pos[0])
		*(d_pos) = -1;
	else
	{
		if (query[0] != '?' && env(query))
		{
			if (env(query)[0] != 0)
				*str = inside_join(*str, env(query) + 1, 1, 0);
			else if ((len = ft_strlen(query) + 1))
				while (++i < len)
					deconechar(*str);
		}
		else if (query[0] == '?')
			*str = inside_join(*str, ft_itoaa(g_mshell.exitcode), 3, 1);
		else
			*str = inside_join(*str, vars(query), 1, 0);
		if (*str == 0)
			return (freeret(query, 1));
	}
	return (0);
}

int		spaghetti_bolognaise(t_list *curr, char *d_pos)
{
	if (curr->nospace == 1)
	{
		if (curr->next && (curr->next->content[0] == '\'' ||
			curr->next->content[0] == '\"'))
		{
			d_pos[0] = 0;
			return (2);
		}
	}
	return (1);
}

int		parse_env_ls(char **str, int len, t_list *curr)
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
		else if (d_pos[1] == 0)
			return (spaghetti_bolognaise(curr, d_pos));
		else if (*str[0] != '\"')
			curr->rawtext = 1;
		while (ft_isalnum(d_pos[len + 1]))
			len++;
		if (len == 0 && d_pos[len + 1] == '?')
			len++;
		if ((query = ft_substr(d_pos, 1, len)) == 0)
			return ((int)(*str = (char *)(long)freeret(*str, 0)));
		if (parse_env_ls_two(query, str, d_pos))
			return (0);
		free(query);
	}
	return (1);
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
	while (str && str[++j])
	{
		if ((str[j] == '\'' || str[j] == '\"'))
		{
			v = 0;
			if ((v % 2 == 0) && !(cpt[str[j] == '\"'] % 2))
				cpt[!(str[j] == '\"')]++;
		}
	}
}
