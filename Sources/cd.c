/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 19:33:13 by edal--ce          #+#    #+#             */
/*   Updated: 2020/08/05 20:02:51 by hpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*getcwdwrap(void)
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

int		cdminus(char *str)
{
	char	*oldpwd;
	char	*pwd;

	if (ft_strcmp(str, "-") != 0)
		return (ft_printh(2, 1, "minishell: cd: %s: invalid option\n", str));
	if ((oldpwd = env("OLDPWD")) == NULL || *oldpwd == 0)
		return (ft_printh(2, 1, "minishell: cd: OLDPWD not set\n"));
	++oldpwd;
	if ((pwd = getcwdwrap()) == NULL)
		return (ft_printh(2, 1, "minishell: cd: %s\n", strerror(errno)));
	if (chdir(oldpwd) == -1)
	{
		free(pwd);
		return (ft_printh(2, 1, "minishell: cd: %s: %s\n", oldpwd, strerror(errno)));
	}
	ft_printh(1, 0, "%s\n", oldpwd);
	if ((oldpwd = ft_strjoin("PWD=", oldpwd)) == NULL)
		ft_printh(2, 0, "minishell: cd: %s\n", strerror(errno));
	else
		export(oldpwd);
	free(oldpwd);
	if ((pwd = ft_strjoinf2("OLDPWD=", pwd)) == NULL)
		ft_printh(2, 0, "minishell: cd: %s\n", strerror(errno));
	else
		export(pwd);
	free(pwd);
	return (0);
}

int		cdbis(void)
{
	t_list	*curr;
	char	*str;

	curr = g_mshell.ls->next;
	if (curr && curr->type == 1)
	{
		if (curr->content[0] == '-')
			return (cdminus(curr->content));
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
		if ((str = rethomedir()) == NULL)
			return (ft_printh(2, 1, "minishell: cd: $HOME not set\n"));
		if (chdir(str)== -1)
			return (ft_printh(2, 1, "minishell: cd: %s: %s\n", str, strerror(errno)));
	}
	return (-1);
}

int		cdter(int pwd)
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

int		cd(void)
{
	char	*pwd;
	int		x;
	int		ret;

	ret = 0;
	if ((pwd = getcwdwrap()) == NULL)
	{
		ret = ft_printh(2, 1, "minishell: cd: %s\n", strerror(errno));
		unset("OLDPWD");
	}
	if ((x = cdbis()) != -1)
	{
		free(pwd);
		return (x);
	}
	if (pwd)
	{
		if ((pwd = ft_strjoinf2("OLDPWD=", pwd)) == NULL)
		{
			ret = ft_printh(2, 1, "minishell: cd: %s\n", strerror(errno));
			unset("OLDPWD");
		}
		else
			export(pwd);
	}
	free(pwd);
	if ((pwd = getcwdwrap()) != NULL)
	{
		if ((pwd = ft_strjoinf2("PWD=", pwd)) == NULL)
		{
			ret = ft_printh(2, 1, "minishell: cd: %s\n", strerror(errno));
			unset("PWD");
		}
		else
			export(pwd);
	}
	else
	{
		ret = ft_printh(2, 1, "minishell: cd: %s\n", strerror(errno));
		unset("PWD");
	}
	free(pwd);
	return (ret);
}
