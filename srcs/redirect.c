/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 23:37:08 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/03 13:06:05 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_file(char *name, int flags)
{
	int	fd;

	if ((fd = open(name, flags, 0644)) < 0)
	{
		ft_dprintf(2, "minishell: %s: %s\n", name, strerror(errno));
		exit(1);
	}
	return (fd);
}

void		enable_redirect(t_cmd *cmd)
{
	t_tok *toks;

	toks = cmd->tok_lst;
	while (toks && toks->next)
	{
		if (toks->type == CHR_RE)
		{
			if (!ft_strcmp(toks->value, ">>"))
				cmd->fdout = handle_file(toks->next->value, O_RDWR | O_CREAT | O_APPEND);
			else if (!ft_strcmp(toks->value, ">"))
				cmd->fdout = handle_file(toks->next->value, O_TRUNC | O_RDWR | O_CREAT);
			else if (!ft_strcmp(toks->value, "<"))
				cmd->fdin = handle_file(toks->next->value, O_RDONLY);
			toks = tok_list_remove(&cmd->tok_lst, toks);
			toks = tok_list_remove(&cmd->tok_lst, toks);
		}
		else
			toks = toks->next;
	}
}