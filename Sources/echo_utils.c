/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 16:22:26 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/08 16:47:25 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	swap_char(char *str, char c)
{
	int		i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == -1)
			str[i] = c;
	}
}

int		check_exitcode(char **str)
{
	char	*pos;
	char	*tmp;

	pos = ft_strnstr(*str, "$?", ft_strlen(*str));
	tmp = ft_itoaa(g_mshell.exitcode);
	if (tmp == 0)
	{
		free(*str);
		*str = 0;
		return (1);
	}
	if (pos)
	{
		*str = inside_join(pos, tmp, 0);
		if (*str == 0)
			return (1);
		pos = ft_strnstr(*str, "$?", ft_strlen(*str));
	}
	else
		free(tmp);
	return (0);
}

void	parse_env_ls(char **str)				//PROTECTED AND LEAK FREE
{
	char	*d_pos;
	char	*query;
	int		len;

	len = 0;
	if (check_exitcode(str))
		return ;
	d_pos = ft_strchr(*str, '$');
	while (d_pos != NULL)
	{
		while (ft_isalnum(d_pos[len + 1]))
			len++;
		// query = ft_substr(d_pos, 1, len);
		if((query = ft_substr(d_pos, 1, len)) == 0)
		{
			free(*str);
			*str = 0;
			return ;
		}
		if (*query == 0 || ft_isspace(*query))
			*(d_pos) = -1;
		else
		{
			if (env(query))
				*str = inside_join(*str, env(query) + 1, 1);
			else
				*str = inside_join(*str, vars(query), 1);
			if(*str == 0)
			{	
				free(query);
				return ;
			}
		}
		d_pos = ft_strchr(*str, '$');
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

void	parse_qts(char *str, int *cpt)
{
	int		j;
	int		v;

	j = -1;
	while (str[++j])
		if ((str[j] == '\'' || str[j] == '\"'))
		{
			v = 0;
			if (j > 0 && str[j - 1] == '\\' && (v = 1))
			{
				while (j - v >= 0 && str[j - v] == '\\')
					v++;
				v--;
			}
			if ((v % 2 == 0) && !(cpt[str[j] == '\"'] % 2))
				cpt[!(str[j] == '\"')]++;
		}
}
