/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 23:55:26 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/02/12 02:15:59 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tok	*create_tok(int type, void *value)
{
	t_tok	*tok;

	tok = (t_tok*)malloc(sizeof(t_tok));
	tok->type = type;
	tok->value = value;
	tok->next = NULL;
	tok->prev = NULL;
	return tok;
}

void	add_token_list(t_tok **begin, t_tok *new)
{
	t_tok	*tok;

	if (!new || !begin)
		return ;
	if (*begin)
	{
		tok = *begin;
		while (tok->next)
		{
			tok->next->prev = tok;
			tok = tok->next;
		}
		tok->next = new;
		new->prev = tok;
		tok->next = NULL;
	}
    else
        *begin = new;
}

void	add_front_tok_list(t_tok **begin, t_tok *new)
{
	if (!new || !begin)
		return ;
	if (*begin)
	{
		(*begin)->prev = new;
		new->next = (*begin);
	}
		*begin = new;
}

int		tok_list_size(t_tok   *tok_lst)
{
    int		i;

	i = 0;
	while (tok_lst)
	{
		tok_lst = tok_lst->next;
		i++;
	}
	return (i);
}