#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <signal.h>
# include <stdio.h>
# include <errno.h>
# include <sys/wait.h>

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
t_dlist	*dlist_create_from_tab(char **tab);
void	dlist_print(t_dlist *begin, int fd);
char	**tab_copy(char **tab);
t_dlist *dlist_strchr_first(t_dlist *begin);
t_dlist	*dlist_chr_alpha_next(t_dlist *begin);
int		built_in_env(int ac, char **av, t_dlist *envlist, int fd);
int     built_in_export(int ac, char **av, t_dlist *envlist, int fd);

//libft a verifier
char	*insert_string(char *str, char *to_insert, int start, int end);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strndup(char *str, int n);



#endif
