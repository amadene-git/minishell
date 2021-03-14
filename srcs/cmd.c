/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 18:43:56 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/14 16:43:19 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_tok	**get_cmd_new(t_tok **tok_lex,  t_cmd *cmd)
{
	int		i;
	char	*s1 = NULL;
	char	*s2 = NULL;
	t_tok	*token = NULL;

	i = 0;
	if ((tok_lex[i]->type >= CHR_WORD && tok_lex[i]->type <= CHR_SP) || tok_lex[i]->type == CHR_RE || tok_lex[i - 1]->type == CHR_RE)
	{
		while (tok_lex[i]->type == CHR_SP)
			i++;
		while (tok_lex[i]->type >= CHR_WORD && tok_lex[i]->type <= CHR_ST)
		{
			if (tok_lex[i]->type == CHR_ST)
			{
				s2 = ft_strndup(tok_lex[i]->value + 1, ft_strlen(tok_lex[i]->value) - 2);
			}
			else if (tok_lex[i]->type == CHR_STR)
			{
				s2 = get_str(ft_strndup(tok_lex[i]->value + 1, ft_strlen(tok_lex[i]->value) - 2), cmd->envlist);
			}
			else if (tok_lex[i]->type == CHR_WORD)
			{
				s2 = get_word(ft_strdup(tok_lex[i]->value), cmd->envlist);
			}
			s1 = ft_strjoindoublefree(s1, s2);
			i++;
		}
		while ((tok_lex[i]->type == CHR_SP) && (s1 == NULL))
			i++;
		if (s1)
			token = create_tok(CHR_WORD, s1);
		else if (tok_lex[i]->type == CHR_RE)
		{
			token = create_tok(CHR_RE, ft_strdup(tok_lex[i]->value));
			i++;
		}
		tok_lex = get_cmd_new(tok_lex + i, cmd);
	}
	else
		return (tok_lex + i);
	tok_list_prepend(&cmd->tok_lst, token);
	return (tok_lex);
}

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