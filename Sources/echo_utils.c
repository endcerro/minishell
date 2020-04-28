/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 16:22:26 by edal--ce          #+#    #+#             */
/*   Updated: 2020/04/28 16:43:02 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *inside_join(char *base, char *add)
{
	int i;
	int j;
	char *out;

	out = malloc(sizeof(char) * (ft_strlen(base) + ft_strlen(add) + 1));
	j = 0;
	i = 0;

	add++;
	// printf("base = %s\n add = %s\n",base,add);

	while(base[i])
	{
		if(j == 0 && base[i] == '$')
		{

			// while(add[j] != '=')
			// 	j++;
			while(add[j])
			{
				// printf("\n");
				// printf("|%c|%d", add[j], i + j);
				out[i + j] = add[j];
				j++;
			}
			i++;
			// printf("current i char = %c\n",base[i] );
			while(ft_isupper(base[i]))
				i++;
		}
		else
		{
			// printf("|%c|%d", base[i],i + j);
			out[i + j] = base[i];
		}
		i++;
	}
	out[i + j] = 0;
	free(base);
	// printf("output  |%s|\n", out);
	return out;
}

void parse_env(char **param, char **envi)
{
	int i;
	int len;
	char *tmp;
	char *tmp2;
	i = 0;
	len = 0;

	tmp = ft_strchr(*param, '$');
	// printf("found at |%s|\n",tmp );
	if (tmp)
	{
		// tmp++;
		while (ft_isupper(tmp[len + 1]))
			len++;
		// printf("len = %d\n",len );
		tmp = ft_substr(tmp, 1, len);
		tmp2 = env(envi, param, tmp);
		*param = inside_join(*param, tmp2 + 1);
		// printf("found at |%s|\n",tmp );
	}
}