/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_two.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <admadene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 17:47:42 by admadene          #+#    #+#             */
/*   Updated: 2021/03/18 09:55:45 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		gevt_one(const char *value, t_tok **tok_var, int *i, int *j)
{
	t_tok	*token;
	char	*str;

	if (ft_isspace(value[(*i)]))
	{
		token = create_tok(CHR_SP, NULL);
		if (!token_push_back(tok_var, token))
			return (0);
		while (ft_isspace(value[(*i)]))
			(*i)++;
	}
	else
	{
		(*j) = (*i);
		while (value[(*j)] && !ft_isspace(value[(*j)]))
			(*j)++;
		str = ft_strndup(value + (*i), (*j) - (*i));
		if (!str)
			return (0);
		token = create_tok(CHR_ST, str);
		if (!token_push_back(tok_var, token))
			return (0);
		(*i) = (*j);
	}
	return (1);
}

t_tok	*get_env_var_tok(const char *value)
{
	t_tok	*tok_var;
	int		i;
	int		j;

	tok_var = NULL;
	if (!value)
	{
		tok_var = create_tok(CHR_ST, NULL);
		return (tok_var);
	}
	if (!*value)
	{
		tok_var = create_tok(CHR_ST, ft_strdup(""));
		return (tok_var);
	}
	i = 0;
	while (value[i])
	{
		gevt_one(value, &tok_var, &i, &j);
	}
	return (tok_var);
}

t_tok	*get_word_tok(t_tok *tok_lex, t_cmd *cmd)
{
	char	*str;
	t_tok	*tok_word;

	if (!(str = gwt_five(tok_lex, cmd)))
		return (NULL);
	tok_word = NULL;
	while (str[cmd->i])
	{
		if (str[cmd->i] == '\\')
		{
			if (!(str = insert_string(str, strdup(""), cmd->i, cmd->i + 1)))
				return (NULL);
		}
		else if (str[cmd->i] == '$')
		{
			if (!gwt_three(str, &cmd->j, &cmd->i))
				continue;
			if (!gwt_one(str, cmd, &tok_word) || !gwt_two(cmd, &tok_word))
				return (NULL);
		}
		cmd->i++;
	}
	if (!gwt_four(str, &tok_word, cmd->i, cmd->k))
		return (NULL);
	return (tok_word);
}

