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
		// while (ft_isupper(cache[len + 1]))
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

	printf("line b4 %s\n", line);
	if (cpt[0] % 2 || cpt[1] % 2)
	{
	 	fill = getfiller(0, cpt); // PAS PROTEGE+
	}


	// parse_env(&line);
	printf("line af %s\n", line);
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
	// else
	// 	out = line;
	// if(out)
	// 	ft_strjoinft(line,out);
	// 	out = line
	printf("return = %s\n",out );
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

void	get_lst(char *line)
{
	char *filler;
	
	filler = check_finished_lst(line);

	if (filler)
	{
		// printf("here\n");
		line = ft_strjoinft(line, filler);
	}
	
	printf("Line b4 env= %s\n",line );
	parse_env(&line);
	printf("Line af env= %s\n",line );


	t_list *out = split_line_lst(line);
	// free(line);
	ft_lstprint(out);
}