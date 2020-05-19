#include "minishell.h"

void	ft_lstprint(t_list *lst)
{
	if (lst)
	{
		while (lst->next)
		{
			ft_putchar('|');
			ft_putstr(lst->content);
			ft_putchar('|');
			// (*f)(lst->content);
			lst = lst->next;
		}
			ft_putchar('|');
			ft_putstr(lst->content);
			ft_putchar('|');
	}
}

void	ft_lstdelone(t_list *lst)
{
	free(lst->content);
	free(lst);
}

void	ft_lstclear(t_list **lst)
{
	t_list *cache;
	t_list *cache2;

	cache = *lst;
	while (cache->next)
	{
		cache2 = cache->next;
		ft_lstdelone(cache);
		cache = cache2;
	}
	ft_lstdelone(cache);
	*lst = 0;
}

t_list	*ft_lstnew(char *content)
{
	t_list *out;

	if (!(out = malloc(sizeof(t_list))))
		return (0);
	out->content = content;
	out->next = 0;
	return (out);
}

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list *tmp;

	if (alst && new)
	{
		tmp = *alst;
		if (tmp)
		{
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
		else
			*alst = new;
	}
}

char	*get_word_lst(char *line, int *p)
{
	size_t	len;
	int		cp;

	cp = *p;
	len = 0;
	if (line[*p] == '"' && ++(*p))
	{
		while (line[*p] && line[*p] != '"' && ++len)
			++(*p);
		++(*p);
		return(ft_substr(line, cp, len + 2));
	}
	else if (line[*p] == '\'' && ++(*p))
	{
		while (line[*p] && line[*p] != '\'' && ++len)
			++(*p);
		++(*p);
		return(ft_substr(line, cp, len + 2));
	}
	else
		while (line[*p] && !ft_isspace(line[*p]) && ++len)
			++(*p);
	return(ft_substr(line, cp, len));
}

void	parse_env_lst(char **param)
{
	int		len;
	char	*tmp;
	char	*cache;
	char	*str;

	len = 0;
	cache = ft_strchr(*param, '$');
	while(cache != NULL)
	{
		while (cache[len + 1] && cache[len + 1] != '=')
			len++;
		tmp = ft_substr(cache, 1, len);
		str = env(tmp);
		*param = inside_join(*param, str == NULL ? vars(tmp) : str);
		free(tmp);
		cache = ft_strchr(*param, '$');
	}
}
char	*check_finished_lst(char *line)	//Chnager le char ** en char * ?
{
	int		i;
	int		cpt[2];
	char	**fill;

	i = -1;
	fill = 0;
	ft_bzero(cpt, 2);
	cpt[0] = 0;
	cpt[1] = 0;

	parse_qts(line, cpt);

	// printf("line b4 %s\n", line);
	if (cpt[0] % 2 || cpt[1] % 2)
	{
	 	fill = getfiller(0, cpt); // PAS PROTEGE+
	}


	// parse_bs(line);

	char *out = 0;
	i = 0;
	if(fill && fill[i])
	{
		out = ft_strdup(fill[i++]);
		while(fill[i])
		{
			out = ft_strjoinf1(out, "\n");
			out = ft_strjoinf1(out, fill[i++]);
		}
	}
	return (out);
}

t_list *split_line_lst(char *line)
{
	t_list 	*f_lst;
	t_list 	*lst;
	int 	i;

	i = 0;
	f_lst = 0;
	while ((size_t)i < ft_strlen(line))
	{
		if (ft_isspace(line[i]))
			++i;
		else
		{
			lst = ft_lstnew(get_word_lst(line, &i));
			if (f_lst == 0)
			{
				f_lst = lst;	
				lst = 0;
			}
			else
				ft_lstadd_back(&f_lst, lst);
		}
	}
	return (f_lst);
}

void tag_lst(t_list *lst)
{
	//Function that needs to set id to know what the text is supposed to be
	//1 for function name
	//2 for params
	//3 for redirection marker
	//etc
}

void	get_lst(char *line)
{
	char *filler;
	
	filler = check_finished_lst(line);

	if (filler)
	{
		// printf("here\n");
		line = ft_strjoinft(line, filler);
	}
	
	// printf("Line b4 env= %s\n",line );
	parse_env(&line);
	// printf("Line af env= %s\n",line );


	t_list *out = split_line_lst(line);
	tag_lst(out);
	// free(line);
	ft_lstprint(out);
	ft_putstr("\n\n");
	g_mshell.ls = out;
}

void	echo_ls()	//Devrait etre pas mal, à vérifier
{
	int		i;
	int		ret;
	t_list	*curr;

	i = 0;
	ret = 0;
	// fill = 0;
	curr = g_mshell.ls->next;
	if (curr && ft_strcmp(curr->content, "-n") == 0)
		curr = curr->next;
	else
		ret = 1;
	// fill = check_finished();


	// char *test = 0;
	// test = check_finished1();
	// curr = g_mshell.ls->
	while (curr)
	{
		ft_putstr(curr->content);
		if (curr->next)
			write(1," ",1);
		curr = curr->next;
	}
	i = -1;
	// while (fill && fill[++i])
	// {
	// 	parse_env(&(fill[i]));
	// 	ft_putstr(fill[i]);
	// 	if (fill[i + 1])
	// 		write(1, "\n", 1);
	// }
	// if(test)
	// 	ft_putstr(test);
	// freechar2ptr(fill);
	if (ret)
		write(1, "\n", 1);
}

void	checkinput_ls(void)
{
	// printf("CHECKING INPUT %s\n",g_mshell.params[0] );
	// check_command(0);
	if (ft_strcmp(g_mshell.ls->content, "exit") == 0) // Fini
	{
		ft_putstr("exit\n");
		if (g_mshell.ls->next)
			ft_putstr("minishell: exit: too many arguments\n");
		freechar2ptr(g_mshell.env);
		// freechar2ptr(g_mshell.params);
		freechar2ptr(g_mshell.vars);
		exit(0);
	}
	else if (ft_strcmp(g_mshell.ls->content, "echo") == 0) // A terminer
	{
		printf("echo ls\n");
		echo_ls();
	}	
	// else if (ft_strcmp(g_mshell.params[0], "env") == 0) // Fini
	// 	env(NULL);
	// else if (ft_strcmp(g_mshell.params[0], "cd") == 0) // Fini // PAS PROTEGE
	// 	cd();
	// else if (ft_strcmp(g_mshell.params[0], "pwd") == 0) // Fini
	// 	pwd();
	// else if (ft_strcmp(g_mshell.params[0], "export") == 0) // A fignoler // PAS PROTEGE
	// 	export(g_mshell.params);
	// else if (ft_strcmp(g_mshell.params[0], "unset") == 0) // A terminer
	// 	unset();
	// else if (ft_strcmp(g_mshell.params[0], "clear") == 0)
	// 	ft_putstr("\033c");
	// else
	// 	commandorvar();
}