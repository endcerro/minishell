/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 19:33:13 by edal--ce          #+#    #+#             */
/*   Updated: 2020/06/08 17:30:58 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*getcwdwrap(void)				//MALLOC PROTECTED
{
	char			*str;
	unsigned int	i;

	i = 256;
	str = NULL;
	while (i < UINT_MAX)
	{
		if (!(str = (char *)malloc(i)))
			return (NULL);
		if (getcwd(str, i) != NULL)
			break ;
		free(str);
		i += 100;
	}
	return (str);
}

char	*rethomedir(void)
{
	int	x;

	x = 0;
	while (g_mshell.env[x] && ft_strncmp("HOME=", g_mshell.env[x], 5) != 0)
		++x;
	if (g_mshell.env[x])
		return (g_mshell.env[x] + 5);
	return (NULL);
}

int		pwd(void)
{
	char *str;

	if (g_mshell.ls->next && g_mshell.ls->next->type == 1)
	{
		ft_putstr("minishell: pwd: too many arguments\n");
		return (1);
	}
	if ((str = getcwdwrap()) == NULL)
	{
		ft_putstr("minishell: pwd: ");
		ft_putstr(strerror(errno));
		return (1);
	}
	ft_putstr(str);
	write(1, "\n", 1);
	free(str);
	return (0);
}

int		cd(void)
{
	int		oldpwd;
	int		pwd;
	int		x;
	char	*home;
	char	*str;
	t_list	*curr;

	curr = g_mshell.ls->next;
	oldpwd = -1;
	pwd = -1;
	x = -1;
	if (curr && curr->type == 1 && curr->next != 0 && curr->next->type == 1)
	{
		ft_putstr("minishell: cd: wrond number of arguments\n");
		return (1);
	}
	if (curr && curr->type == 1)
	{
		if (curr->content[0] == '~')
		{
			curr->content[0] = '/';
			if (!(curr->content = ft_strjoinf2(rethomedir(), curr->content)))
				return (1);
		}
		if (chdir(curr->content) == -1)
		{
			ft_putstr(strerror(errno));
			write(1, "\n", 1);
			return (1);
		}
	}
	else
		if (chdir((home = rethomedir())) == -1)
		{
			ft_putstr("minishell: cd: $HOME not set\n");
			return (1);
		}
	while (g_mshell.env[++x] && (pwd == -1 || oldpwd == -1))
		if (ft_strncmp(g_mshell.env[x], "PWD=", 4) == 0)
			pwd = x;
		else if (ft_strncmp(g_mshell.env[x], "OLDPWD=", 7) == 0)
			oldpwd = x;
	if (g_mshell.env[x] == NULL)
		return (0);
	free(g_mshell.env[oldpwd]);
	g_mshell.env[oldpwd] = ft_strjoin("OLD", g_mshell.env[pwd]); // FREE F2?
	if (g_mshell.env[oldpwd] == 0)
	{
		return (1);
	}
	free(g_mshell.env[pwd]);
	g_mshell.env[pwd] = (char *) -1;		//SKIP THIS IN FREECHAR2PTR if issue on getcwdwrap
	if ((str = getcwdwrap()) == NULL)
		ft_putstr(strerror(errno));
	else
		g_mshell.env[pwd] = ft_strjoinf2("PWD=", str); // PAS PROTEGE
	return (0);
}
