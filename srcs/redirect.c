/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 23:37:08 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/02/10 00:47:45 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_file(t_cmd *cmd, int flags)
{
	int	fd;

	if ((fd = open(cmd->av[0], flags, 0644)) < 0)
	{
		dprintf(2, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
	}
	return (fd);
}

void	enable_redirect(t_tok **tok_lex)
{
	t_cmd *cmd;

	cmd = (t_cmd*)malloc(sizeof(t_cmd));
	if (ft_strcmp((*tok_lex)->value, ">") && ft_strcmp((*(tok_lex + 1))->value, ">"))
	{
		while ((*tok_lex)->type == CHR_SP || (*tok_lex)->type == CHR_OP)
			tok_lex++;
		tok_lex = get_cmd(tok_lex, cmd, 0);
		handle_file(cmd, O_RDWR | O_CREAT | O_APPEND);
	}
	else if (ft_strcmp((*tok_lex)->value, ">"))
	{
		while ((*tok_lex)->type == CHR_SP || (*tok_lex)->type == CHR_OP)
			tok_lex++;
		tok_lex = get_cmd(tok_lex, cmd, 0);
		handle_file(cmd, O_TRUNC | O_RDWR | O_CREAT);
	}
	else if (ft_strcmp((*tok_lex)->value, "<"))
	{
		while ((*tok_lex)->type == CHR_SP || (*tok_lex)->type == CHR_OP)
			tok_lex++;
		tok_lex = get_cmd(tok_lex, cmd, 0);
		handle_file(cmd, O_RDONLY);
	}
}