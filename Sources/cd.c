/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 19:33:13 by edal--ce          #+#    #+#             */
/*   Updated: 2020/07/10 21:03:14 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*getcwdwrap(void)						//PROTECTED
{
	char			*str;
	unsigned int	i;

	i = 256;
	str = NULL;
	while (i < UINT_MAX)
	{
		if (!(str = (char *)xmalloc(i)))
			return (NULL);
		if (getcwd(str, i) != NULL)
			break ;
		free(str);
		i += 100;
	}
	return (str);
}

char	*rethomedir(void)						//PROTECTED
{
	int	x;

	x = 0;
	while (g_mshell.env[x] && ft_strncmp("HOME=", g_mshell.env[x], 5) != 0)
		++x;
	if (g_mshell.env[x])
		return (g_mshell.env[x] + 5);
	return (NULL);
}

int		pwd(void)								//PROTECTED
{
	char *str;

	if ((str = getcwdwrap()) == NULL)
		return (ft_printh(2, 1, "minishell: pwd: %s\n", strerror(errno)));
	ft_putstr(str);
	write(1, "\n", 1);
	free(str);
	return (0);
}

int		cdbis(void)								//PROTECTED
{
	t_list	*curr;

	curr = g_mshell.ls->next;
	if (curr && curr->type == 1)
	{
		if (curr->content[0] == '~')
		{
			curr->content[0] = '/';
			if (!(curr->content = ft_strjoinf2(rethomedir(), curr->content)))
				return (1);
		}
		if (chdir(curr->content) == -1)
			return (ft_printh(2, 1, "minishell: cd: %s: %s\n", curr->content,
							strerror(errno)));
	}
	else
	{
		if (chdir(rethomedir()) == -1)
			return (ft_printh(2, 1, "minishell: cd: $HOME not set\n"));
	}
	return (0);
}

int		cdter(int pwd)							//PROTECTED
{
	char *str;

	if ((str = getcwdwrap()) == NULL)
		return (ft_printh(2, 1, "minishell: cd: %s\n", strerror(errno)));
	else
	{
		if ((g_mshell.env[pwd] = ft_strjoinf2("PWD=", str)) == NULL)
		{
			g_mshell.env[pwd] = (char *)-1;
			return (ft_printh(2, 1, "minishell: cd: %s\n", strerror(errno)));
		}
	}
	return (0);
}

int		cd(void)								//PROTECTED
{
	int		oldpwd;
	int		pwd;
	int		x;

	if (cdbis())
		return (1);
	oldpwd = -1;
	pwd = -1;
	x = -1;
	while (g_mshell.env[++x] && (pwd == -1 || oldpwd == -1))
		if (ft_strncmp(g_mshell.env[x], "PWD=", 4) == 0)
			pwd = x;
		else if (ft_strncmp(g_mshell.env[x], "OLDPWD=", 7) == 0)
			oldpwd = x;
	if (g_mshell.env[x] == NULL)
		return (0);
	free(g_mshell.env[oldpwd]);
	g_mshell.env[oldpwd] = ft_strjoin("OLD", g_mshell.env[pwd]);
	if (g_mshell.env[oldpwd] == 0)
		return (ft_printh(2, 1, "minishell: cd: $PWD problem\n"));
	free(g_mshell.env[pwd]);
	g_mshell.env[pwd] = (char *)-1;
	return (cdter(pwd));
}
