#ifndef MINISHELL_H
# define MINSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct	s_btree
{
	struct s_btree	*right;
	struct s_btree	*left;
	struct s_btree	*parent;
	void			*item;
}				t_btree;

char **ft_split_whitespaces(char *str);
void	ft_putstr(char *str);
void	ft_putchar(char c);
#endif
