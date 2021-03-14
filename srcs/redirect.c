/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 23:37:08 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/14 23:00:10 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int		run_file(char *name, int flags, int *status)
{
	int	fd;

	if ((fd = open(name, flags, 0644)) < 0)
	{
		ft_dprintf(2, "minishell: %s: %s\n", name, strerror(errno));
		*status = 1;
	}
	return (fd);
}

static t_tok	*get_file_name(t_tok *toks, char **file_name)
{
	t_tok	*tmp;
	char	*tmp_str;

	tmp = toks->next;
	while (tmp->type == CHR_SP)
		tmp = tmp->next;
	*file_name = ft_strnew(0);
	while (tmp->type == CHR_ST)
	{
		tmp_str = *file_name;
		*file_name = ft_strjoin(*file_name, tmp->value);
		ft_memdel((void **)&tmp_str);
		tmp = tmp->next;
	}
	return (tmp);
}

int				enable_redirect(t_cmd *cmd)
{
	t_tok	*toks;
	t_tok	*tmp;
	char	*n;
	int		status;

	toks = cmd->tok_arg;
	status = 0;
	while (toks && toks->next)
		if (toks->type == CHR_RE)
		{
			tmp = get_file_name(toks, &n);
			if (!ft_strcmp(toks->value, ">>"))
				cmd->fdout = run_file(n, O_RDWR | O_CREAT | O_APPEND, &status);
			else if (!ft_strcmp(toks->value, ">"))
				cmd->fdout = run_file(n, O_TRUNC | O_RDWR | O_CREAT, &status);
			else if (!ft_strcmp(toks->value, "<"))
				cmd->fdin = run_file(n, O_RDONLY, &status);
			while (toks != tmp)
				toks = tok_list_remove(&cmd->tok_arg, toks);
			ft_memdel((void **)&n);
		}
		else
			toks = toks->next;
	return (status);
}
