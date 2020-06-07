/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 19:21:47 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/07 19:32:32 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int check_valid_export(char **params) //TODO
{

	return (1);
}

int		check_match(char *env, char *param)		//PROTECTED
{
	int ret;
	int i ;

	i = 0;
	// printf("strlen = %d\n",ft_strlen(ft_strnstr(env, "=", ft_strlen(env) )));

	while (env[i] && env[i] != '=')
		i++;


	ret = ft_strncmp(env, param, i);

	// printf("ret = %d\n",ret );

	return (ret);
}
