#include "minishell.h"

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

char	*check_finished_lst(char *line)	//Chnager le char ** en char * ?
{
	int		i;
	int		cpt[2];
	char	**fill;
	char 	*out;

	i = -1;
	fill = 0;
	ft_bzero(cpt, 2);
	cpt[0] = 0;
	cpt[1] = 0;
	parse_qts(line, cpt);

	if (cpt[0] % 2 || cpt[1] % 2)
	{
	 	fill = getfiller(0, cpt); // PAS PROTEGE+
	}
	out = 0;
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



void check_rdir()
{
	t_list *curr;
	int fd;

	curr = g_mshell.ls;

	while(curr && curr->type != 3)
	{
		if (curr->type == 2 && curr->next && curr->next->type == 1)
		{
			g_mshell.oldfd = dup(1);
			fd = open(curr->next->content,
			 O_APPEND | O_TRUNC | O_WRONLY | O_CREAT, 0644);
			dup2(fd, 1);
		}
		curr = curr->next;
	}
}

int		parse_esc(char *str)
{
	int i;
	int j;
	int r;
	int tra;

	r = 0;
	tra = 0;
	j = -1;
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