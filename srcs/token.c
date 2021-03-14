/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 23:55:26 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/14 16:31:24 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	tok_list_append(t_tok **begin, t_tok *new)
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

void	tok_list_prepend(t_tok **begin, t_tok *new)
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

int		tok_list_size(t_tok *tok_lst)
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

t_tok	*tok_list_remove(t_tok **begin, t_tok *tok)
{
	t_tok	*temp;

	temp = *begin;
	while (temp != NULL)
	{
		if (temp == tok)
		{
			if (temp->next == NULL)
			{
				if (temp->prev == NULL)
					*begin = NULL;
				else
					temp->prev->next = NULL;
			}
			else if (temp->prev == NULL)
			{
				temp->next->prev = NULL;
				*begin = temp->next;
			}
			else
			{
				temp->next->prev = temp->prev;
				temp->prev->next = temp->next;
			}
			ft_memdel((void **)&tok->value);
			free(tok);
			return (temp->next);
		}
		else
			temp = temp->next;
	}
	return (NULL);
}