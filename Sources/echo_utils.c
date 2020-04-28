/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 16:22:26 by edal--ce          #+#    #+#             */
/*   Updated: 2020/04/28 17:40:45 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *inside_join(char *base, char *add)
{
	int i;
	int j;
	int p;
	char *out;

	// printf("%p\n",add );
	if(add != 0)
		out = malloc(sizeof(char) * (ft_strlen(base) + ft_strlen(add) + 1));
	else
		out = malloc(sizeof(char) * (ft_strlen(base) + 1));
	p = 0;
	j = 0;
	i = 0;

	// if(add)
	// 	add++;
	// printf("base = %s\n add = %s\n",base,add);

	while(base[i])
	{
		if(j == 0 && base[i] == '$')
		{
			// printf("here %s\n",base + i);
			while(add && add[j])
				out[p++] = add[j++];
			i++;
			while(++j && ft_isupper(base[i]))
				i++;
			i--;
		}
		else
		{
			// printf("|%c|\n",base[i] );
			out[p++] = base[i];
		}
		i++;
	}
	out[p] = 0;
	free(base);
	return out;
}

void parse_env(char **param, char **envi)
{
	int i;
	int len;
	char *tmp;
	char *tmp2;
	char *cache;

	i = 0;
	len = 0;
	// printf("Param = %s\n",*param);
	cache = ft_strchr(*param, '$');
	// printf("cache = %s\n",cache );
	
	while(cache != NULL)
	{
		if (cache)
		{
			while (ft_isupper(cache[len + 1]))
				len++;
			tmp = ft_substr(cache, 1, len);
			tmp2 = env(envi, param, tmp);
			if(tmp2)
				*param = inside_join(*param, tmp2);
			else
				*param = inside_join(*param, 0);
		}
		// printf("param = %s\n",*param );
		cache = ft_strchr(*param, '$');
		// printf("cache = %s\n",cache );
		// cache = ft_strchr(cache, '$');
		// printf("new param %s\n",cache );
	}
}