/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 14:34:58 by admadene          #+#    #+#             */
/*   Updated: 2021/03/17 14:16:01 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_tok	**get_tok_arg(t_tok **tok_lex, t_cmd *cmd)
{
	char 	*str;
	t_tok	*token;

	if (!tok_lex || !(*tok_lex))
		return (NULL);
	while ((*tok_lex)->type > CHR_ERROR && (*tok_lex)->type < CHR_PV && (*tok_lex)->type != CHR_PI)
		{
			if ((*tok_lex)->type == CHR_SP)
			{
				token = create_tok(CHR_SP, NULL);
				if (token)
				{
					if (!token_push_back(&cmd->tok_arg, token))
						return (NULL);
				}
				else
					return (NULL);
			}
			else if ((*tok_lex)->type == CHR_ST)
			{
				str = ft_strndup((*tok_lex)->value + 1, ft_strlen((*tok_lex)->value) - 2);
				if (str)
				{
					token = create_tok(CHR_ST, str);
					if (token)
						token_push_back(&cmd->tok_arg, token);
					else	
						return (NULL);
				}
				else
					return (NULL);
				
			}
			else if ((*tok_lex)->type == CHR_STR)
			{
				str = get_str(ft_strndup((*tok_lex)->value + 1, ft_strlen((*tok_lex)->value) - 2), cmd->envlist);
				if (str)
				{
					token = create_tok(CHR_ST, str);
					if (token)
						token_push_back(&cmd->tok_arg, token);
					else
						return (NULL);
				}
				else
					return (NULL);				
			}
			else if ((*tok_lex)->type == CHR_WORD)
			{
				token = get_word_tok(*tok_lex, cmd);
				if (token)
					token_push_back(&cmd->tok_arg, token);
				else
					return (NULL);
			}
			else
			{
				token = create_tok((*tok_lex)->type, ft_strdup((*tok_lex)->value));
				if (token)
					token_push_back(&cmd->tok_arg, token);
				else
					return (NULL);
				
			}			
			tok_lex++;
		}
	token_push_back(&cmd->tok_arg, create_tok(CHR_END, ft_strdup("newline")));
	return (tok_lex);
}

void	get_ac_av(t_tok *tok_lst,  t_cmd *cmd, int lvl)
{
	char	*s1 = NULL;
	char	*s2 = NULL;

	if (!tok_lst)
		return;
	if (tok_lst->type != CHR_END && tok_lst->type > CHR_ERROR &&  tok_lst->type < CHR_OP)
	{
		while (tok_lst->type == CHR_SP)
			tok_lst = tok_lst->next;
		while (tok_lst && tok_lst->type == CHR_ST)
		{
			if (tok_lst->value)
			{
				s2 = ft_strdup((char*)tok_lst->value);
				s1 = ft_strjoindoublefree(s1, s2);
			}
			tok_lst = tok_lst->next;
		}
		if (s1 && tok_lst->type != CHR_END && tok_lst->type > CHR_ERROR &&  tok_lst->type < CHR_OP)
			get_ac_av(tok_lst->next, cmd, lvl + 1);
		else if (s1)
			get_ac_av(tok_lst, cmd, lvl + 1);
		else
			get_ac_av(tok_lst, cmd, lvl);

	}
	else
	{
		cmd->av = (char**)malloc(sizeof(char*) * (lvl + 1));
		cmd->av[lvl] = NULL;
		cmd->ac = lvl;
		return;
	}
	if (s1)
		cmd->av[lvl] = s1;
	return;
}
