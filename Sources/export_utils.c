/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 19:21:47 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/26 14:01:52 by edal--ce         ###   ########.fr       */
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


	while (env[i] && env[i] != '=')
		i++;


	ret = ft_strncmp(env, param, i);



	return (ret);
}
