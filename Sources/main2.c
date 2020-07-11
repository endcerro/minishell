/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/11 14:44:08 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/11 22:39:02 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
void	promptnextline(char **str, char *cpt)
{
	char c;
	char baise[2];

	write(1, "dquote> ", 8);
	if (get_next_line(0, str) == 0)
	{
		c = cpt[0] % 2 ? '\"' : '\'';
		baise[0] = c;
		baise[1] = 0;
		ft_printh(2, 1, "bash: unexpected EOF while looking for matching `%s\'\nbash: syntax error: unexpected end of file", baise);
		free(*str);
		*str = NULL;
	}
}

char	**getfiller(int depth, char *cpt)
{
	char *tmp;
	char **out;

	promptnextline(&tmp, cpt);
	if (tmp == NULL)
		return (NULL);
	escape_chars(tmp, 0, 0);
	parse_qts(tmp, cpt);
	if (cpt[0] % 2 || cpt[1] % 2 || cpt[2])
	{
		out = getfiller(depth + 1, cpt);
		if (out == 0)
			return ((char **)(long)freeret(tmp, 0));
	}
	else
	{
		if (!(out = malloc(sizeof(char *) * (depth + 2))))
			return ((char **)(long)freeret(tmp, 0));
		out[depth + 1] = 0;
	}
	if (depth == 0)
		if (!(tmp = ft_strjoinf2("\n", tmp)))
			return ((char **)(long)freeret(out, 0));
	out[depth] = tmp;
	return (out);
}

int		checkexport(char *var)
{
	int x;

	x = 0;
	while (var[x] && var[x] != '=')
		++x;
	if (var[x] == 0)
		return (0);
	return (1);
}

char	*env(char *request)
{
	int x;

	x = 0;
	if (request == NULL && g_mshell.ls->next && g_mshell.ls->next->type == 1)
	{
		ft_putstr_fd("minishell: env: too many arguments\n", 2);
		return (NULL);
	}
	while (g_mshell.env[x])
	{
		if (request != NULL)
		{
			if (!ft_strncmp(g_mshell.env[x], request, wordlen(request)))
				return (g_mshell.env[x] + wordlen(request));
		}
		else if (checkexport(g_mshell.env[x]) == 1)
			ft_putsendl(g_mshell.env[x]);
		++x;
	}
	return (NULL);
}

char	*vars(char *request)
{
	int x;

	if (request != NULL && g_mshell.vars != NULL)
	{
		x = 0;
		while (g_mshell.vars[x])
		{
			if (!ft_strncmp(g_mshell.vars[x], request, ft_strlen(request)))
				return (g_mshell.vars[x] + ft_strlen(request) + 1);
			++x;
		}
	}
	return (NULL);
}
