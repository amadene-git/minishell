/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 20:17:36 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/17 00:41:52 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			handle_signal(int signo)
{
	if (signo == SIGINT)
		ft_putstr_fd("\n[minishell]>", 1);
	if (signo == SIGQUIT)
		ft_dprintf(2, "\b\b  \b\b");
}

t_dlist			*stock_env_status(int status, t_dlist *envlist)
{
	t_var	*var;
	t_dlist	*elem;

	var = (t_var*)malloc(sizeof(t_var));
	var->name = ft_strdup("?");
	var->value = ft_itoa(status);
	if ((elem = dlist_chr(envlist, var->name)))
	{
		free(elem->data->value);
		elem->data->value = ft_strdup(var->value);
		free_var(var);
	}
	else
		envlist = insert_var(envlist, var);
	return (envlist);
}

int				has_pipe(t_tok **tok_lex)
{
	int	i;

	i = 0;
	while ((tok_lex[i]->type <= CHR_SP || tok_lex[i]->type == CHR_PI))
	{
		if (tok_lex[i]->type == CHR_PI)
			return (1);
		i++;
	}
	return (0);
}

void	refresh_last_cmd(t_dlist *envlist, char *last_cmd)
{
	t_dlist *last;

	if (!(last = dlist_chr(envlist, "_")))
		last = insert_var(envlist, create_var("_="));
	if (last->data->value)
		free(last->data->value);
	last->data->value = last_cmd;
}

void	free_lexer(t_tok **tok_lex, int lvl)
{
	if (!tok_lex)
		return ;
	if (tok_lex[lvl + 1] && tok_lex[lvl + 1]->type < CHR_END)
		free_lexer(tok_lex, lvl + 1);
	else
	{
		if (tok_lex[lvl + 1])
		{
			if (tok_lex[lvl + 1]->value)
				free(tok_lex[lvl + 1]->value);
			free(tok_lex[lvl + 1]);
		}
		if (tok_lex[lvl]->value)
			free(tok_lex[lvl]->value);
		free(tok_lex[lvl]);
		if (!lvl)
			free(tok_lex);
		return ;
	}
	if (tok_lex[lvl]->value)
		free(tok_lex[lvl]->value);
	free(tok_lex[lvl]);
	if (!lvl)
		free(tok_lex);
}

void	free_av(char **av, int lvl)
{
	if (!av)
		return ;
	if (av[lvl] && av[lvl + 1])
		free_av(av, lvl + 1);
	free(av[lvl]);
	if (!lvl)
		free(av);
}

void	free_cmd_lst(t_cmd *cmd)
{
	t_cmd *tmp;

	if (!cmd)
		return ;
	while (cmd->next)
		cmd = cmd->next;
	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->prev;
		free(tmp);
	}
}

static void	init_signal(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
}

static int	prepare_execution(t_tok ***tok_lex, t_cmd *cmd, t_dlist *envlist,\
			int status)
{
	t_tok **tmp_lex;

	tmp_lex = *tok_lex;
	while ((*tmp_lex)->type == CHR_SP\
	|| (*tmp_lex)->type == CHR_OP\
	|| (*tmp_lex)->type == CHR_PI || (*tmp_lex)->type == CHR_PV)
		tmp_lex++;
	*tok_lex = tmp_lex;
	cmd->env = get_env_from_envlist(envlist, envlist, 0);
	envlist = stock_env_status(status, envlist);
	*tok_lex = get_tok_arg(*tok_lex, cmd);
	status = enable_redirect(cmd);
	if (has_pipe(*tok_lex) == 1)
	{
		cmd->fdpipe = (int*)malloc(sizeof(int) * 2);
		if (pipe(cmd->fdpipe) == -1)
			ft_dprintf(2, "erreur main:%s\n", strerror(errno));
	}
	get_ac_av(cmd->tok_arg, cmd, 0);
	return (status);
}

static int	handle_execution(t_cmd *cmd, int status)
{
	if (cmd && cmd->tok_arg && cmd->av && cmd->av[0] && !status)
	{
		cmd->bin = ft_strdup(cmd->av[0]);
		get_absolute_path(cmd, cmd->envlist);
		status = exec_no_fork(cmd);
		if (status == 0)
			status = exec_cmd(cmd);
	}
	return (status);
}

int	main(int ac, const char **av, const char **env)
{
	char	*line;
	int		gnl;
	int		k;
	t_tok	**tok_lex;
	t_tok	**save_lex;
	t_dlist	*envlist;
	t_cmd	*cmd;
	t_cmd	*tmp;
	int		status;

	init_signal();
	status = 0;
	gnl = 1;
	tmp = NULL;
	envlist = init_env(env);
	while (gnl)
	{
		line = NULL;
		if (ac == 1)
		{
			ft_dprintf(1, "[minishell]>");
			gnl = get_next_line(0, &line);
		}
		else if (ac > 2)
			line = ft_strdup(av[2]);
		if (gnl && line && *line)
		{
			k = 0;
			tok_lex = lexer(line, &k, 0);
			save_lex = tok_lex;
			cmd = NULL;
			if (!has_errors(tok_lex))
				while ((*tok_lex)->type != CHR_END)
				{
					cmd = prepare_cmd(envlist, tmp, save_lex, line);
					status = prepare_execution(&tok_lex, cmd, envlist, status);
					status = handle_execution(cmd, status);
					tmp = cmd;
					envlist = stock_env_status(status, envlist);
					free_av(cmd->env, 0);
					free_av(cmd->av, 0);
					free_tok_arg(cmd->tok_arg);
					free(cmd->bin);
				}
			else
				status = 2;
			free_lexer(save_lex, 0);
		}
		if (ac != 1)
			gnl = 0;
		free(line);
	}
	free_cmd(cmd);
	free_envlist(envlist);
	if (ac == 1)
		ft_dprintf(1, "exit\n");
	exit(status);
}
