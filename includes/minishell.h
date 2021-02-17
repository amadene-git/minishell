#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <signal.h>
# include <stdio.h>
# include <errno.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <dirent.h>
#include <fcntl.h>

//get_next_line defines
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 128
# endif

# ifndef OPEN_MAX
#  define OPEN_MAX 256
# endif

# ifndef PATHSIZE
#  define PATHMAX 32000
# endif

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


typedef struct s_tok
{
    void            *value;
    int             type;
    struct s_tok    *prev;
    struct s_tok    *next;
}               t_tok;

typedef struct  s_cmd
{
    int				ac;
    char    		**av;
    char		    **env;
    t_dlist			*envlist;
    char            *bin;
    int				fdin;
	int				fdout;
    int             fdpipe[2];
    pid_t           pid;
    t_tok           *tok_lst;
    struct  s_cmd   *prev;
    struct  s_cmd   *next;
}               t_cmd;

enum e_chr{
	CHR_ERROR,
	CHR_WORD,
	CHR_STR,
	CHR_ST,
	CHR_SP,
	CHR_OP,
    CHR_PI,
    CHR_RE,
	CHR_END
};

typedef struct  s_minishell
{
    char    *line;
    int     index;
    t_cmd   *cmd_lst;
}               t_minishell;

//built_in_echo.c
int		built_in_echo(int ac, char **av, t_dlist *envlist, int fd);

//built_in_cd.c
void	refresh_pwd(const char *pwd, const char *old, t_dlist *envlist, int fd);
int		built_in_cd(int ac, char **av, t_dlist *envlist, int fd);

//built_in_pwd.c
int		built_in_pwd(int ac, char **av, t_dlist *envlist, int fd);

//built_in_export.c
void	print_export(t_dlist *envlist, int fd);
int     built_in_export(int ac, char **av, t_dlist *envlist, int fd);

//built_in_env.c
char	*str_plusplus(char *nbr);
t_dlist	*init_env(const char **env);
t_dlist *get_envlist_from_env(const char **env);
void    envlist_print(const char **env, int fd);
int     built_in_unset(t_cmd *cmd);
char	*get_var_str(t_var *var);
char	**get_env_from_envlist(t_dlist *begin, t_dlist *elem, int lvl);
int     built_in_env(t_cmd *cmd);

//exec.c
void		get_absolute_path(char **cmd, t_dlist *envlist);
int			is_builtin(char	*cmd);
void		exec_built_in(t_cmd *cmd);
int 		exec_bin(t_cmd *cmd);

//parser.c
t_tok	**lexer(char *str, int *i, int lvl);
char	*put_var_env(char *str, t_dlist *envlist);
char	*get_st(char *str);
char	*get_str(char *str, t_dlist *envlist);
t_tok	**get_cmd(t_tok **tok_lex,  t_cmd *cmd, int lvl);
char	*get_word(char *str, t_dlist *envlist);

//dlist.c
t_dlist *dlist_create_elem(void *data);
t_var	*create_var(const char *str);
t_dlist *dlist_strchr_first(t_dlist *begin);
t_dlist	*dlist_chr_alpha_next(t_dlist *begin);
void	insert_var(t_dlist *envlist, t_var *variable);
t_dlist	*dlist_chr(t_dlist *begin, const char *name);
void    free_var(t_var *variable);
void	free_elem(t_dlist *envlist, const char *name);
void    free_envlist(t_dlist *envlist);

//redirect.c
void    enable_redirect(t_cmd *cmd);

//token.c
t_tok	*create_tok(int type, void *value);
void	tok_list_append(t_tok **begin, t_tok *new);
void    tok_list_prepend(t_tok **begin, t_tok *new);
int		tok_list_size(t_tok *tok_lst);
void    tok_list_remove(t_tok **begin, t_tok *tok);

//cmd.c
t_tok	**get_cmd_new(t_tok **tok_lex,  t_cmd *cmd);
char	**to_char_args(t_tok *tok_lst);

//libft a verifier
char	*insert_string(char *str, char *to_insert, int start, int end);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strjoindoublefree(char *s1, char *s2);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strndup(const char *str, int n);
void	ft_putstr_fd(char const *s, int fd);
void	ft_putchar_fd(char c, int fd);
char	*ft_strdup(const char *s1);
int		ft_isalpha(int c);
int		get_next_line(int const fd, char **line);
size_t	ft_strlen(const char *s);
void	ft_putendl_fd(char const *s, int fd);
char	*ft_strchr(const char *s, int c);
void	*ft_calloc(size_t count, size_t size);
char	**ft_split(char const *s, char c);
int		ft_tolower(int c);
#endif
