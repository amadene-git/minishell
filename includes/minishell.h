#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <signal.h>
# include <stdio.h>
# include <errno.h>
# include <sys/wait.h>

typedef struct  s_var
{
    char    *name;
    char    *value;
}               t_var;

typedef struct  s_dlist
{
    struct s_dlist  *next;
    struct s_dlist  *prev;
    t_var           *data;
    
}               t_dlist;

//exec.c
char		**split(char *raw_cmd, char *limiti);
void		free_array(char **array);
void		get_absolute_path(char **cmd);
int			is_builtin(char	*cmd);
void		exec_built_in(int ac, char **cmd, t_dlist *envlist, int fd);
void		exec_cmd(char **cmd, char **env);

//parser.c
char	*get_quotes(const char *str, int *i);
char	*get_dquotes(const char *str, int *i);
int		is_sep_expr(char c);
char	*get_expr(const char *str, int *i);
char	*get_next_word(const char *str, int *i);
char	**split_cmdline(const char *str, int *i, int n);

//built_in.c
int		built_in_echo(int ac, char **av, t_dlist *envlist, int fd);
int		built_in_cd(int ac, char **av, t_dlist *envlist, int fd);
int		built_in_pwd(int ac, char **av, t_dlist *envlist, int fd);

//built_in_export.c
t_dlist *dlist_create_elem(void *data);
t_var	*create_var(const char *str);
t_dlist	*dlist_create_from_tab(const char **tab);
void	dlist_print(t_dlist *begin, int fd);
t_dlist *dlist_strchr_first(t_dlist *begin);
t_dlist	*dlist_chr_alpha_next(t_dlist *begin);
void	print_export(t_dlist *envlist, int fd);
void	insert_var(t_dlist *envlist, t_var *variable);
int     built_in_export(int ac, char **av, t_dlist *envlist, int fd);
t_dlist	*dlist_chr(t_dlist *begin, const char *name);
void    free_var(t_var *variable);
void	free_elem(t_dlist *envlist, const char *name);
int		built_in_unset(int ac, char **av, t_dlist *envlist, int fd);
int		built_in_env(int ac, char **av, t_dlist *envlist, int fd);

//libft a verifier
char	*insert_string(char *str, char *to_insert, int start, int end);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strndup(const char *str, int n);



#endif
