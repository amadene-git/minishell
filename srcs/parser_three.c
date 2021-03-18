/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_three.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <admadene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 17:50:40 by admadene          #+#    #+#             */
/*   Updated: 2021/03/18 09:57:40 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		gwt_three(const char *str, int *j, int *i)
{
	(*j) = (*i) + 1;
	while (ft_isalnum(str[(*j)]) || str[(*j)] == '_' || str[(*j)] == '?')
		(*j)++;
	if ((*j) == (*i) + 1)
	{
		(*i)++;
		return (0);
	}
	return (1);
}

int		gwt_one(const char *str, t_cmd *cmd, t_tok **tok_word)
{
	if (cmd->i - cmd->k > 0 && (!token_push_back(tok_word,\
	create_tok(CHR_ST, ft_strndup(str + cmd->k, cmd->i - cmd->k)))))
		return (0);
	cmd->tmp = (void*)ft_strndup(str + cmd->i + 1, cmd->j - (cmd->i + 1));
	if (!cmd->tmp)
		return (0);
	return (1);
}

int		gwt_two(t_cmd *cmd, t_tok **tok_word)
{
	t_tok	*token;
	t_dlist	*elem;

	elem = dlist_chr(cmd->envlist, cmd->tmp);
	free(cmd->tmp);
	if (!elem)
	{
		token = create_tok(CHR_ST, NULL);
		if (!token)
			return (0);
		if (!token_push_back(tok_word, token))
			return (0);
	}
	else
	{
		token = get_env_var_tok((char*)elem->data->value);
		if (!token)
			return (0);
		if (!token_push_back(tok_word, token))
			return (0);
	}
	cmd->i = cmd->j - 1;
	cmd->k = cmd->j;
	return (1);
}

int		gwt_four(char *str, t_tok **tok_word, int i, int k)
{
	t_tok	*token;

	if (i - k > 0)
	{
		token = create_tok(CHR_ST, ft_strndup(str + k, i - k));
		if (!token)
			return (0);
		if (!token_push_back(tok_word, token))
			return (0);
	}
	free(str);
	return (1);
}

char	*gwt_five(t_tok *tok_lex, t_cmd *cmd)
{
	char	*str;

	str = ft_strndup(tok_lex->value, ft_strlen((char*)tok_lex->value));
	if (!str)
		return (NULL);
	cmd->i = 0;
	cmd->k = 0;
	return (str);
}

