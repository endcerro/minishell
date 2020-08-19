/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:49 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/19 20:01:57 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*add_filler(char **fill)
{
	int		i;
	char	*out;

	out = 0;
	i = 0;
	if (fill && fill[i])
	{
		if ((out = ft_strdup(fill[i])) == NULL)
			return ((char *)(long)freechar2ptr(fill, 0));
		while (fill[++i])
		{
			if ((out = ft_strjoinf1(out, "\n")) == NULL)
				break ;
			if ((out = ft_strjoinf1(out, fill[i])) == NULL)
				break ;
		}
	}
	return (out);
}

int		check_end_pipe(char *line, char *cpt)
{
	int i;
	int c;
	char lc;

	c = 0;
	i = -1;
	lc = 0;
	while (line && line[++i])
	{
		// lc = line[i];
		if (line[i] != '|' && !ft_isspace(line[i]) && line[i] != ';')
		{
			lc = line[i];
			cpt[3] = 0;
			c++;
		}
		else if (line[i] == '|' || (cpt[3] && line[i] == ';'))
		{
			if (lc == '>' || lc == ';' || lc == '<' || lc == '|')
				return (1);
			if (c == 0)
			{
				cpt[3]++;
				return (1);
			}
			else
				cpt[3]++;
			c = 0;
		}
		if (line[i] == ';' && line[i + 1] == ';')
			return (1);
		if(!ft_isspace(line[i]))
			lc = line[i];

	}
	if (c == 0 && ft_strchr(line, '|') != 0)
		cpt[3]++;
	return (0);
}

char	*check_finished_lst(char *line, int *err)
{
	char	cpt[4];
	char	**fill;
	char	*out;

	fill = 0;
	out = 0;
	ft_bzero(cpt, 4);
	escape_chars(line, -1, 0, cpt);
	if (check_end_pipe(line, cpt))
		return (0);
	parse_qts(line, cpt);
	if (cpt[0] % 2 || cpt[1] % 2 || cpt[2] || cpt[3])
		if ((fill = getfiller(0, cpt, err)) == NULL)
			return (NULL);
	if ((out = add_filler(fill)) == NULL)
		return (NULL);
	freechar2ptr(fill, 0);
	return (out);
}

char	*getcwdwrap(void)
{
	char			*str;

	str = getcwd(NULL, 0);
	return (str);
}
