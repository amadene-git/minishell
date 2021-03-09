/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 23:37:08 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/09 14:38:19 by mbouzaie         ###   ########.fr       */
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
	t_tok	*toks;
	t_tok	*tmp;
	char	*file_name;
	char	*tmp_str;

	toks = cmd->tok_arg;
	file_name = ft_strnew(0);
	while (toks && toks->next)
	{
		if (toks->type == CHR_RE)
		{
			tmp = toks->next;
			while (tmp->type == CHR_SP)
				tmp = tmp->next;
			while (tmp->type == CHR_ST)
			{
				tmp_str = file_name;
				file_name = ft_strjoin(tmp->value, file_name);
				ft_memdel((void **) &tmp_str);
				tmp = tmp->next;
			}
			if (!ft_strcmp(toks->value, ">>"))
				cmd->fdout = handle_file(file_name, O_RDWR | O_CREAT | O_APPEND);
			else if (!ft_strcmp(toks->value, ">"))
				cmd->fdout = handle_file(file_name, O_TRUNC | O_RDWR | O_CREAT);
			else if (!ft_strcmp(toks->value, "<"))
				cmd->fdin = handle_file(file_name, O_RDONLY);
			while (toks != tmp)
				toks = tok_list_remove(&cmd->tok_arg, toks);
		}
		else
			toks = toks->next;
	}
}