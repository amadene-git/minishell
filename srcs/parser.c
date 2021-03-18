/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 14:34:58 by admadene          #+#    #+#             */
/*   Updated: 2021/03/18 12:22:32 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		gta_six(t_tok *tok_lex, t_cmd *cmd)
{
	t_tok *token;

	token = create_tok(tok_lex->type, ft_strdup(tok_lex->value));
	if (token)
		token_push_back(&cmd->tok_arg, token);
	else
		return (0);
	return (1);
}

t_tok	**get_tok_arg(t_tok **tok_lex, t_cmd *cmd)
{
	int		i;

	if (!tok_lex || !(*tok_lex))
		return (NULL);
	while ((*tok_lex)->type > CHR_ERROR && (*tok_lex)->type < CHR_PV &&\
			(*tok_lex)->type != CHR_PI)
	{
		i = gta_five(tok_lex, cmd);
		if (i == -1)
			return (NULL);
		else if (i == 0)
		{
			tok_lex++;
			continue;
		}
		else
		{
			if (!gta_six(*tok_lex, cmd))
				return (NULL);
		}
		tok_lex++;
	}
	token_push_back(&cmd->tok_arg, create_tok(CHR_END, ft_strdup("newline")));
	return (tok_lex);
}

char	*gaa(t_tok *tok_lst, t_cmd *cmd, int *lvl)
{
	char	*s1;
	char	*s2;

	s1 = NULL;
	s2 = NULL;
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
	if (s1 && tok_lst->type != CHR_END && tok_lst->type > CHR_ERROR &&\
			tok_lst->type < CHR_OP)
		get_ac_av(tok_lst->next, cmd, *lvl + 1);
	else if (s1)
		get_ac_av(tok_lst, cmd, *lvl + 1);
	else
		get_ac_av(tok_lst, cmd, *lvl);
	return (s1);
}

void	get_ac_av(t_tok *tok_lst, t_cmd *cmd, int lvl)
{
	char	*s1;

	s1 = NULL;
	if (!tok_lst)
		return ;
	if (tok_lst->type != CHR_END && tok_lst->type > CHR_ERROR &&\
			tok_lst->type < CHR_OP)
		s1 = gaa(tok_lst, cmd, &lvl);
	else
	{
		cmd->av = (char**)malloc(sizeof(char*) * (lvl + 1));
		cmd->av[lvl] = NULL;
		cmd->ac = lvl;
		return ;
	}
	if (s1)
		cmd->av[lvl] = s1;
	return ;
}
