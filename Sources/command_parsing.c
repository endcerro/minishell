/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/26 16:28:49 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/22 15:14:25 by edal--ce         ###   ########.fr       */
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

int		check_end_pipe2(char last, char *cpt, int i, char *line)
{
	if (last == '|')
	{
		if (cpt[3])
			return (1);
		cpt[3] = 1;
	}
	else if (last == '\\' && cpt[3])
		return (line[i] = 0);
	else if (last != -1 && last != '|')
		cpt[3] = 0;
	return (0);
}

int		check_end_pipe(char *line, char *cpt, int i, int last)
{
	while (line[++i])
	{
		if (last == -1 && line[i] == '|')
			return (1);
		if (last == '|' && line[i] == '|')
			return (1);
		else if (last == ';' && line[i] == '|')
			return (1);
		else if (last == '>' && line[i] == '|')
			return (1);
		else if (last == '<' && line[i] == '|')
			return (1);
		if (!ft_isspace(line[i]))
		{
			if (last == '|' && line[i] == '\\' && ++cpt[3])
				line[i] = ' ';
			else if (line[i] != '|')
				cpt[3] = 0;
			last = line[i];
		}
	}
	return (check_end_pipe2(last, cpt, i, line));
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
	if (check_end_pipe(line, cpt, -1, -1))
		return (0);
	parse_qts(line, cpt);
	if (cpt[0] % 2 || cpt[1] % 2 || cpt[2] || cpt[3])
		if ((fill = getfiller(0, cpt, err, 0)) == NULL)
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
