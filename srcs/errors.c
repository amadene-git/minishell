/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/18 13:07:52 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/03 13:07:00 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_error(t_tok *actual, t_tok *prev)
{
	if (actual->type == CHR_ERROR)
		return (0);
	if ((actual->type == CHR_PI || actual->type == CHR_OP) && !prev)
		return (0);
	if (actual->type == CHR_RE && (prev
		&& prev->type == CHR_RE))
		return (0);
	if (actual->type == CHR_END && prev
		&& (prev->type == CHR_RE || prev->type == CHR_PI))
		return (0);
	if ((actual->type == CHR_PI || actual->type == CHR_OP)
		&& (prev->type == CHR_PI || prev->type == CHR_OP
		|| prev->type == CHR_RE))
		return (0);
	return (1);
}

int	has_errors(t_tok **tok_lex)
{
	int		i;
	int		j;

	i = -1;
	j = 0;
	while (tok_lex[++i] && tok_lex[i]->type <= CHR_END)
	{
		while (tok_lex[i] && tok_lex[i]->type == CHR_SP)
			i++;
		if (tok_lex[i] && ((i == 0 && !check_error(tok_lex[i], NULL)) || !check_error(tok_lex[i], tok_lex[j])))
		{
			ft_dprintf(2, "minishell: syntax error near unexpected token `%s'\n", (char *)tok_lex[i]->value);
			return (1);
		}
		if (tok_lex[i] && tok_lex[i]->type == CHR_END)
			return (0);
		j = i;
	}
	return (0);
}