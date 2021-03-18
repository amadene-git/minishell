/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 23:09:02 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/18 11:11:19 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include "libft.h"
# include "ft_dprintf.h"

# ifndef PATHSIZE
#  define PATHSIZE 32000
# endif

typedef struct	s_var
{
	char	*name;
	char	*value;
}				t_var;

typedef struct	s_dlist
{
	struct s_dlist	*next;
	struct s_dlist	*prev;
	t_var			*data;
}				t_dlist;

typedef struct	s_tok
{
	void			*value;
	int				type;
	struct s_tok	*prev;
	struct s_tok	*next;
}				t_tok;

typedef struct	s_cmd
{
	int				ac;
	char			**av;
	char			**env;
	t_dlist			*envlist;
	char			*bin;
	int				fdin;
	int				fdout;
	int				*fdpipe;
	pid_t			pid;
	t_tok			*tok_arg;
	struct s_cmd	*prev;
	struct s_cmd	*next;
	t_tok			**tok_lex;
	char			*line;
	void			*tmp;
	int				i;
	int				j;
	int				k;
}				t_cmd;

enum			e_chr{
	CHR_ERROR,
	CHR_WORD,
	CHR_STR,
	CHR_ST,
	CHR_SP,
	CHR_OP,
	CHR_PI,
	CHR_RE,
	CHR_PV,
	CHR_END
};

typedef struct	s_minishell
{
	int		gnl;
	int		status;
	char	*line;
	t_cmd	*cmd;
	t_tok	**tok_lex;
	t_dlist	*envlist;
}				t_minishell;

typedef void	(*sighandler_t)(int);

void			built_in_echo(int ac, char **av, int fd);
void			refresh_pwd(char *newpwd, t_dlist *envlist);
int				built_in_cd(int ac, char **av, t_dlist *envlist);
int				built_in_pwd(int ac, char **av);
int				print_export(t_dlist *envlist, int fd);
int				built_in_export(int ac, char **av, t_dlist *envlist, int fd);
char			*str_plusplus(char *nbr);
t_dlist			*init_env(const char **env);
t_dlist			*get_envlist_from_env(const char **env);
void			envlist_print(const char **env, int fd);
int				built_in_unset(t_cmd *cmd);
char			*get_var_str(t_var *var);
char			**get_env_from_envlist(t_dlist *begin, t_dlist *elem, int lvl);
int				built_in_env(t_cmd *cmd);
int				exit_cmd(t_cmd *cmd);
void			get_absolute_path(t_cmd *cmd, t_dlist *envlist);
int				is_builtin(char	*cmd);
int				exec_built_in(t_cmd *cmd);
int				exec_bin(t_cmd *cmd);
int				exec_no_fork(t_cmd *cmd);
int				has_errors(t_tok **tok_lex);
t_tok			**lexer(char *str, int *i, int lvl);
char			*get_var(char *str, int *i, int *j, t_dlist *envlist);
char			*get_st(char *str);
char			*get_str(char *str, t_dlist *envlist);
int				token_push_back(t_tok **begin, t_tok *token);
t_tok			**get_cmd(t_tok **tok_lex, t_cmd *cmd, int lvl);
int				exec_cmd(t_cmd *cmd);
void			handle_signal(int signo);
char			*get_word(char *str, t_dlist *envlist);
void			get_ac_av(t_tok *tok_lst, t_cmd *cmd, int lvl);
t_tok			**get_tok_arg(t_tok **tok_lex, t_cmd *cmd);
t_tok			*get_env_var_tok(const char *value);
int		gevt_one(const char *value, t_tok **tok_var, int *i, int *j);
t_tok			*get_word_tok(t_tok *tok_lex, t_cmd *cmd);
int				gwt_three(const char *str, int *j, int *i);
int				gwt_one(const char *str, t_cmd *cmd, t_tok **tok_word);
int				gwt_two(t_cmd *cmd, t_tok **tok_word);
int				gwt_four(char *str, t_tok **tok_word, int i, int k);
char			*gwt_five(t_tok *tok_lex, t_cmd *cmd);
t_dlist			*dlist_create_elem(void *data);
t_var			*create_var(const char *str);
t_dlist			*dlist_strchr_first(t_dlist *begin);
t_dlist			*dlist_chr_alpha_next(t_dlist *begin);
t_dlist			*insert_var(t_dlist *envlist, t_var *variable);
t_dlist			*dlist_chr(t_dlist *begin, const char *name);
void			free_var(t_var *variable);
void			free_elem(t_dlist *envlist, const char *name);
void			free_envlist(t_dlist *envlist);
int				enable_redirect(t_cmd *cmd);
t_tok			*create_tok(int type, void *value);
int				tok_list_size(t_tok *tok_lst);
t_tok			*tok_list_remove(t_tok **begin, t_tok *tok);
t_cmd			*prepare_cmd(t_dlist *envlist, t_cmd *prev, t_tok **tok_lex,\
							char *line);
void			free_cmd(t_cmd *cmd);
char			*insert_string(char *str, char *to_insert, int start, int end);
char			*ft_strjoindoublefree(char *s1, char *s2);
int				is_number(char *str);
int				ft_strcmpci(const char *s1, const char *s2);
int				is_minus_one(const char *str);
int				is_zero(const char *str);
int				is_valid_name(char *str);
void			free_av(char **av, int lvl);
void			free_lexer(t_tok **tok_lex, int lvl);
void			free_tok_arg(t_tok *tok_arg);
char			g_get_chr(char c);
t_tok			**lexer(char *str, int *i, int lvl);
void			clean_spaces(t_dlist *shlvl);
void			init_signal(void);
int				has_pipe(t_tok **tok_lex);
t_dlist			*stock_env_status(int status, t_dlist *envlist);
int				gta_one(t_cmd *cmd);
int				gta_two(t_tok *tok_lex, t_cmd *cmd);
int				gta_three(t_tok *tok_lex, t_cmd *cmd);
int				gta_four(t_tok **tok_lex, t_cmd *cmd);
int				gta_five(t_tok **tok_lex, t_cmd *cmd);


#endif
