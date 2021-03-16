/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 18:43:56 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/17 00:30:39 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd			*prepare_cmd(t_dlist *envlist, t_cmd *prev, t_tok **tok_lex,\
							char *line)
{
	t_cmd	*cmd;

	cmd = (t_cmd*)malloc(sizeof(t_cmd));
	cmd->envlist = envlist;
	cmd->fdin = -1;
	cmd->fdout = -1;
	cmd->fdpipe = NULL;
	cmd->prev = (prev) ? prev : NULL;
	cmd->next = NULL;
	cmd->tok_arg = NULL;
	cmd->bin = NULL;
	cmd->tok_lex = tok_lex;
	cmd->line = line;
	cmd->av = NULL;
	if (prev)
	{
		prev->line = NULL;
		prev->next = cmd;
	}
	return (cmd);
}

void			free_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;

	if (cmd)
		while (cmd)
		{
			tmp = cmd;
			cmd = cmd->prev;
			if (tmp->fdpipe)
				free(tmp->fdpipe);
			free(tmp);
		}
}

static void		handle_fd(t_cmd *cmd)
{
	if (cmd->prev && cmd->prev->fdpipe)
	{
		dup2(cmd->prev->fdpipe[0], STDIN_FILENO);
		close(cmd->prev->fdpipe[1]);
	}
	if (cmd->fdpipe)
	{
		cmd->fdin != -1 || cmd->fdout != -1 ? exit(1) : 0;
		dup2(cmd->fdpipe[1], STDOUT_FILENO);
		close(cmd->fdpipe[0]);
	}
	if (cmd->fdout != -1)
	{
		dup2(cmd->fdout, STDOUT_FILENO);
		close(cmd->fdout);
	}
	if (cmd->fdin != -1)
	{
		dup2(cmd->fdin, STDIN_FILENO);
		close(cmd->fdin);
	}
}

void			handle_pid_zero(t_cmd *cmd, int *status)
{
	handle_fd(cmd);
	if (!ft_strcmp("exit", cmd->bin))
		*status = 0;
	else if (cmd->bin && !is_builtin(cmd->bin) && (*status = exec_bin(cmd)))
		exit(*status);
	else
		exit(exec_built_in(cmd));
}

int				exec_cmd(t_cmd *cmd)
{
	int		status;

	status = 0;
	cmd->pid = fork();
	if (cmd->pid == -1)
		ft_putstr_fd(strerror(errno), 2);
	if (cmd->pid > 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (cmd->pid == 0)
		handle_pid_zero(cmd, &status);
	if (cmd->prev && cmd->prev->fdpipe)
	{
		close(cmd->prev->fdpipe[0]);
		close(cmd->prev->fdpipe[1]);
	}
	waitpid(cmd->pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == 3)
		ft_dprintf(2, "Quitter (core dumped)\n");
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
	status = WEXITSTATUS(status);
	return (status);
}
