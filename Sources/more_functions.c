/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/11 14:36:41 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/11 14:42:28 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		pwd(void)
{
	char *str;

	if ((str = getcwdwrap()) == NULL)
		return (ft_printh(2, 1, "minishell: pwd: %s\n", strerror(errno)));
	ft_putstr(str);
	write(1, "\n", 1);
	free(str);
	return (0);
}

void	swap_char(char *str, char c)
{
	int		i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == -1)
			str[i] = c;
	}
}

int		parse_esc(char *str, int j)
{
	int		i;
	int		r;
	int		tra;

	r = 0;
	tra = 0;
	if (*(str + 1) == '\\')
	{
		while (*(str + r) == '\\')
			r++;
		if (*(str + r) == '"' || *(str + r) == '\'')
			r++;
		tra = (r % 2 == 0) ? r - r / 2 : r - (r + 1) / 2;
		r = -tra;
	}
	else if ((*(str + 1) == '\'' || *(str + 1) == '\"') && (tra = 1))
		*str = (*(str + 1) == '\'') ? '\'' : '\"';
	while (++j < tra)
	{
		i = -1;
		while (str[++i])
			str[i] = str[i + 1];
	}
	return (r);
}

void	ft_lstdelone(t_list *lst)
{
	free(lst->content);
	free(lst);
}

int		isstrdigit(char *str)
{
	while (*str && (*str == '-' || *str == '+'))
		++str;
	while (*str)
	{
		if (ft_isdigit(*str) == 0)
			return (0);
		++str;
	}
	return (1);
}
