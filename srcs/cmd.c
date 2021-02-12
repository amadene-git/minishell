/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 18:43:56 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/02/12 03:51:34 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_tok	**get_cmd_new(t_tok **tok_lex,  t_cmd *cmd)
{
	int		i;
	int		j;
	char	*str;
	char	*s1 = NULL;
	char	*s2 = NULL;
	t_tok	*token;

	i = 0;
	j = 0;
	if ((tok_lex[i]->type >= CHR_WORD && tok_lex[i]->type <= CHR_SP) || tok_lex[i]->type == CHR_RE || tok_lex[j]->type == CHR_RE)
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
		while (tok_lex[i]->type == CHR_SP)
			i++;
		if (tok_lex[i]->type == CHR_RE)
		{
			token = create_tok(CHR_RE, ft_strdup(tok_lex[i]->value));
			i++;
		}
		else if (tok_lex[i - 1]->type == CHR_RE)
		{
			token = create_tok(CHR_WORD, ft_strdup(tok_lex[i]->value));
			i++;
		}
		else
			token = create_tok(CHR_WORD, s1);
		tok_lex = get_cmd_new(tok_lex + i, cmd);
		j = i - 1;
	}
	else
		return (tok_lex + i);
	add_front_tok_list(&cmd->tok_lst, token);
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
	while (tok_lst && tok_lst->type != CHR_RE)
	{
		args[i] = ft_strdup(tok_lst->value);
		tok_lst = tok_lst->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}
/*
t_cmd	*parse_cmd(t_tok **tok_list, int i)
{
	if ((*tok_list)->type = CHR_WORD)
	{
		
	}
}

t_cmd	*parse_cmd_lst(t_tok **tok_lex)
{
	t_cmd	*cmd;
	int		i;

	if (!(cmd = ft_calloc(1, sizeof(t_cmd))))
		return (NULL);
	i = -1;
	while (tok_lex[++i]->type < CHR_END)
	{
		i++;
	}

}
*/