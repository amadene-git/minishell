/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 23:37:08 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/02/12 04:03:20 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_file(char *name, int flags)
{
	int	fd;

	if ((fd = open(name, flags, 0644)) < 0)
	{
		dprintf(2, "minishell: %s: %s\n", name, strerror(errno));
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (fd);
}

int		enable_redirect(t_tok *cmd_tok)
{
	while (cmd_tok && cmd_tok->next)
	{
		if (!ft_strcmp(cmd_tok->value, ">>"))
		{
			handle_file(cmd_tok->next->value, O_RDWR | O_CREAT | O_APPEND);
			return (1);
		}
		else if (!ft_strcmp(cmd_tok->value, ">"))
		{
			handle_file(cmd_tok->next->value, O_TRUNC | O_RDWR | O_CREAT);
			return (1);
		}
		else if (!ft_strcmp(cmd_tok->value, "<"))
		{
			handle_file(cmd_tok->next->value, O_RDONLY);
			return (1);
		}
		else
			cmd_tok = cmd_tok->next;
	}
	return (0);
}