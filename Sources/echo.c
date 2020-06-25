
#include "minishell.h"

void 	trim_quotes(char *str)
{
	int i;

	i = 1;
	while(str[i])
	{
		str[i - 1] = str[i];
		i++;
	}
	str[i - 2] = 0;
}

int		echo_ls(void)
{
	int		ret;
	t_list	*curr;

	ret = 0;
	curr = g_mshell.ls->next;
	
	if (curr && ft_strcmp(curr->content, "-n") == 0)
		while (curr && ft_strcmp(curr->content, "-n") == 0)
			curr = curr->next;
	else
		ret = 1;
	// if(curr->content[0] == '\"' || curr->content[0] == '\'')
	// 	trim_quotes(curr->content);
	while (curr && curr->type == 1)
	{
		ft_putstr(curr->content);
		if (curr->next)
			write(1, " ", 1);
		curr = curr->next;
	}
	if (ret)
		write(1, "\n", 1);
	return (0);
}