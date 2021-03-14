/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 18:43:56 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/14 22:17:08 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**to_char_args(t_tok *tok_lst)
{
	int		i;
	char	**args;

	i = 0;
	if (!(args = (char **)ft_calloc(1, sizeof(char *) *
		(tok_list_size(tok_lst) + 1))))
		return NULL;
	while (tok_lst && tok_lst->value)
	{
		args[i] = ft_strdup(tok_lst->value);
		tok_lst = tok_lst->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}

void	prepare_cmd(t_cmd *cmd)
{

	//cmd->av = to_char_args(cmd->tok_lst);
	//cmd->ac = tok_list_size(cmd->tok_lst);
	get_absolute_path(cmd, cmd->envlist);
}

void	free_cmd(t_cmd *cmd)
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