/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 16:22:26 by edal--ce          #+#    #+#             */
/*   Updated: 2020/05/19 13:07:57 by edal--ce         ###   ########.fr       */
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
		out = malloc(sizeof(char) * (ft_strlen(base) + 1));
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
	free(base);
	return out;
}

void	parse_env(char **param)
{
	int		len;
	char	*tmp;
	char	*cache;
	char	*str;

	len = 0;
	cache = ft_strchr(*param, '$');
	while(cache != NULL)
	{
		// while (ft_isupper(cache[len + 1]))
		while (cache[len + 1] && cache[len + 1] != '=')
			len++;
		
		tmp = ft_substr(cache, 1, len);
		printf("searching for %s\n",tmp );
		str = env(tmp);
		printf("str found = %s\n",str );
		*param = inside_join(*param, str == NULL ? vars(tmp) : str);
		free(tmp);
		cache = ft_strchr(*param, '$');
	}
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
		parse_env(&g_mshell.params[i]);
		parse_bs(g_mshell.params[i]);
	}
	if (cpt[0] % 2 || cpt[1] % 2)
	 	fill = getfiller(0, cpt); // PAS PROTEGE
	return (fill);
}

char	*check_finished1()	//Chnager le char ** en char * ?
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
		parse_env(&g_mshell.params[i]);
		parse_bs(g_mshell.params[i]);
	}
	if (cpt[0] % 2 || cpt[1] % 2)
	 	fill = getfiller(0, cpt); // PAS PROTEGE+

	char *out = 0;
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

	// add_blocks()

	// freechar2ptr(fill);
	return (out);
}