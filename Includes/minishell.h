#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <unistd.h>
# include <string.h>
# include <errno.h>

#include <stdio.h>


char **get_blocks(char *line);

#endif
