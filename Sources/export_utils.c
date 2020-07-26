/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 19:21:47 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/26 17:20:05 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_match(char *env, char *param)
{
	int ret;
	int i;
	int j;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	j = 0;
	while (param[j] && param[j] != '=')
		++j;
	ret = ft_strncmp(env, param, i < j ? j : i);
	return (ret);
}
