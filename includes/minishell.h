#ifndef MINISHELL_H
# define MINSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "libft.h"


typedef struct	s_btree
{
	struct s_btree	*right;
	struct s_btree	*left;
	struct s_btree	*parent;
	void			*item;
}				t_btree;

#endif
