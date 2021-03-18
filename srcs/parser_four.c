/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_four.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <admadene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 11:47:26 by admadene          #+#    #+#             */
/*   Updated: 2021/03/18 11:55:30 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		gta_one(t_cmd *cmd)
{
	t_tok	*token;

	token = create_tok(CHR_SP, NULL);
	if (token)
	{
		if (!token_push_back(&cmd->tok_arg, token))
			return (0);
	}
	else
		return (0);
	return (1);
}

int		gta_two(t_tok *tok_lex, t_cmd *cmd)
{
	char	*str;
	t_tok	*token;

	str = ft_strndup(tok_lex->value + 1, ft_strlen(tok_lex->value) - 2);
	if (str)
	{
		token = create_tok(CHR_ST, str);
		if (token)
			token_push_back(&cmd->tok_arg, token);
		else
			return (0);
	}
	else
		return (0);
	return (1);
}

int		gta_three(t_tok *tok_lex, t_cmd *cmd)
{
	char	*str;
	t_tok	*token;

	str = get_str(ft_strndup(tok_lex->value + 1,\
	ft_strlen(tok_lex->value) - 2), cmd->envlist);
	if (str)
	{
		token = create_tok(CHR_ST, str);
		if (token)
			token_push_back(&cmd->tok_arg, token);
		else
			return (0);
	}
	else
		return (0);
	return (1);
}

int		gta_four(t_tok **tok_lex, t_cmd *cmd)
{
	t_tok *token;

	token = get_word_tok(*tok_lex, cmd);
	if (token)
		token_push_back(&cmd->tok_arg, token);
	else
		return (0);
	return (1);
}

int		gta_five(t_tok **tok_lex, t_cmd *cmd)
{
	if ((*tok_lex)->type == CHR_SP)
	{
		if (!gta_one(cmd))
			return (-1);
		return (0);
	}
	else if ((*tok_lex)->type == CHR_ST)
	{
		if (!gta_two(*tok_lex, cmd))
			return (-1);
		return (0);
	}
	else if ((*tok_lex)->type == CHR_STR)
	{
		if (!gta_three(*tok_lex, cmd))
			return (-1);
		return (0);
	}
	else if ((*tok_lex)->type == CHR_WORD)
	{
		if (!gta_four(tok_lex, cmd))
			return (-1);
		return (0);
	}
	return (1);
}
