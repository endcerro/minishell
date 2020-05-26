/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 16:22:26 by edal--ce          #+#    #+#             */
/*   Updated: 2020/05/26 16:16:52 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *inside_join(char *base, char *add)
{
	int i;
	int j;
	int p;
	char *out;

	if(add != 0)
		out = malloc(sizeof(char) * (ft_strlen(base) + ft_strlen(add) + 1));
	else
		out = ft_strdup(base);
	p = 0;
	j = 0;
	i = 0;

	while(base[i])
	{
		if(j == 0 && base[i] == '$')
		{
			while(add && add[j])
				out[p++] = add[j++];
			i++;
			while(++j && base[i] && !ft_isspace(base[i]))
				i++;
		}
		else
			out[p++] = base[i++];
	}
	out[p] = 0;
	// free(base);
	return (out);
}

void 	swap_char(char *str, char c)
{
	int		i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == -1)
			str[i] = c;
	}
}

void 	parse_env_ls(char **str) //Gerer le $ seul
{
	char	*$_pos;
	char	*query;
	int		len;

	len = 0;
	$_pos = ft_strchr(*str, '$');
	while($_pos != NULL)
	{
		while ($_pos[len + 1] && ft_isalnum($_pos[len + 1]))
			len++;
		query = ft_substr($_pos, 1, len);
		if(*query == 0 || ft_isspace(*query))
			*($_pos) = -1;
		else
		{
			if(env(query))
				*str = inside_join(*str, env(query) + 1);
			else
				*str = inside_join(*str, vars(query));
		}
		$_pos = ft_strchr(*str, '$');
		free(query);
	}
	swap_char(*str, '$');
}

int		parse_bs(char *str)
{
	int i;

	i = -1;
	while (str[++i])
		if (str[i] == '\\')
			i -= parse_esc(str + i);
	return 0;
}


void 	parse_qts(char *str, int *cpt)
{
	// int 	i;
	int 	j;
	int 	v;

	// i = 0;
	j = -1;
	while (str[++j])
		if ((str[j] == '\'' || str[j] == '\"'))
		{
			v = 0;
			if(j > 0 && str[j - 1] == '\\' && (v = 1))
			{
				while (j - v >= 0 && str[j - v] == '\\')
					v++;
				v--;
			}
			if ((v % 2 == 0) && !(cpt[str[j] == '\"'] % 2))
			{
				cpt[!(str[j] == '\"')]++;
				// i = -1;
				// while(str[j + ++i])
				// {
				// 	str[j + i] = str[j + i + 1];
				// }
				// j--;
			}
		}
}

char	**check_finished()	//Chnager le char ** en char * ?
{
	int		i;
	int		cpt[2];
	char	**fill;

	i = -1;
	fill = 0;
	ft_bzero(cpt, 2);
	cpt[0] = 0;
	cpt[1] = 0;
	while (g_mshell.params[++i])
	{
		parse_qts(g_mshell.params[i], cpt);
		parse_env_ls(&g_mshell.params[i]);
		parse_bs(g_mshell.params[i]);
	}
	if (cpt[0] % 2 || cpt[1] % 2)
	 	fill = getfiller(0, cpt); // PAS PROTEGE
	return (fill);
}