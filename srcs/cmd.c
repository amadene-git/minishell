/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 18:43:56 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/15 22:19:59 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char			**to_char_args(t_tok *tok_lst)
{
	int		i;
	char	**args;

	i = 0;
	if (!(args = (char **)ft_calloc(1, sizeof(char *) *
		(tok_list_size(tok_lst) + 1))))
		return (NULL);
	while (tok_lst && tok_lst->value)
	{
		args[i] = ft_strdup(tok_lst->value);
		tok_lst = tok_lst->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}

void			prepare_cmd(t_cmd *cmd)
{
	get_absolute_path(cmd, cmd->envlist);
}

void			free_cmd(t_cmd *cmd)
{
	int		i;

	while (cmd)
	{
		if (cmd->av)
		{
			i = -1;
			while (cmd->av[++i])
				ft_memdel((void **)&cmd->av[i]);
			free(cmd->av);
		}
		if (cmd->fdpipe)
			free(cmd->fdpipe);
		if (cmd->bin)
			ft_memdel((void **)&cmd->bin);
		while (cmd->tok_arg)
			cmd->tok_arg = tok_list_remove(&cmd->tok_arg, cmd->tok_arg);
		free(cmd);
		cmd = cmd->prev;
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
	{
		handle_fd(cmd);
		if (!ft_strcmp("exit", cmd->bin))
			status = 0;
		else if (cmd->bin && !is_builtin(cmd->bin) && (status = exec_bin(cmd)))
			exit(status);
		else
			exit(exec_built_in(cmd));
	}
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
