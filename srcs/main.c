/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 20:17:36 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/18 11:10:18 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static void	process_minishell(t_minishell *shell, t_tok ***save_lex,\
								t_cmd *pre_cmd)
{
	int	k;

	k = 0;
	shell->tok_lex = lexer(shell->line, &k, 0);
	*save_lex = shell->tok_lex;
	shell->cmd = NULL;
	if (!has_errors(shell->tok_lex))
		while ((*(shell->tok_lex))->type != CHR_END)
		{
			shell->cmd = prepare_cmd(shell->envlist, pre_cmd, *save_lex,
									shell->line);
			shell->status = prepare_execution(&(shell->tok_lex), shell->cmd,
												shell->envlist, shell->status);
			shell->status = handle_execution(shell->cmd, shell->status);
			pre_cmd = shell->cmd;
			shell->envlist = stock_env_status(shell->status, shell->envlist);
			free_av(shell->cmd->env, 0);
			free_av(shell->cmd->av, 0);
			free_tok_arg(shell->cmd->tok_arg);
			free(shell->cmd->bin);
		}
	else
		shell->status = 2;
	free_lexer(*save_lex, 0);
}

static void	init_minishell(t_minishell *shell, const char **env, t_cmd **cmd)
{
	init_signal();
	shell->status = 0;
	shell->gnl = 1;
	shell->envlist = init_env(env);
	shell->line = NULL;
	*cmd = NULL;
}

int			main(int ac, const char **av, const char **env)
{
	t_tok		**save_lex;
	t_cmd		*tmp;
	t_minishell	shell;

	init_minishell(&shell, env, &tmp);
	while (shell.gnl)
	{
		if (ac == 1)
		{
			ft_dprintf(1, "[minishell]>");
			shell.gnl = get_next_line(0, &(shell.line));
		}
		else if (ac > 2)
			shell.line = ft_strdup(av[2]);
		if (shell.gnl && shell.line && *(shell.line))
			process_minishell(&shell, &save_lex, tmp);
		if (ac != 1)
			shell.gnl = 0;
		ft_memdel((void **)&(shell.line));
	}
	free_cmd(shell.cmd);
	free_envlist(shell.envlist);
	if (ac == 1)
		ft_dprintf(1, "exit\n");
	exit(shell.status);
}
