/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 16:22:26 by edal--ce          #+#    #+#             */
/*   Updated: 2020/05/06 19:00:55 by edal--ce         ###   ########.fr       */
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
		str = env(tmp);
		*param = inside_join(*param, str == NULL ? vars(tmp) : str);
		free(tmp);
		cache = ft_strchr(*param, '$');
	}
}
