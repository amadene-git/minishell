/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dlist.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:42:51 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/16 15:46:52 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_dlist	*dlist_create_elem(void *data)
{
	t_dlist *elem;

	if (!(elem = (t_dlist *)malloc(sizeof(t_dlist))))
		return (NULL);
	elem->data = data;
	return (elem);
}

void	print_elem(t_dlist *elem, const char *str)
{
	printf("%s\n", str);
	printf("%s->%p\tprev->%p\tnext->%p\n", (char *)elem->data, elem,\
			elem->prev, elem->next);
}

t_dlist	*dlist_strchr_first(t_dlist *begin)
{
	t_dlist	*elem;
	t_dlist *ret;

	if (!begin)
		return (NULL);
	elem = begin->next;
	ret = begin;
	while (elem && elem != begin)
	{
		if (ft_strcmp((char*)ret->data->name, (char*)elem->data->name) >= 0)
			ret = elem;
		elem = elem->next;
	}
	return (ret);
}

t_dlist	*dlist_chr_alpha_next(t_dlist *begin)
{
	t_dlist	*elem;
	t_dlist *ret;

	if (!begin)
		return (NULL);
	elem = begin->next;
	ret = begin;
	while (elem && elem != begin && ret == begin)
	{
		if (ft_strcmp(begin->data->name, elem->data->name) < 0)
			ret = elem;
		elem = elem->next;
	}
	while (elem && elem != begin)
	{
		if (ft_strcmp(begin->data->name, elem->data->name) < 0 &&\
			ft_strcmp(elem->data->name, ret->data->name) < 0)
			ret = elem;
		elem = elem->next;
	}
	if (ret == begin)
		return (NULL);
	return (ret);
}

t_dlist	*dlist_chr(t_dlist *begin, const char *name)
{
	t_dlist *elem;

	if (!begin)
		return (NULL);
	if (!ft_strcmp(name, begin->data->name))
		return (begin);
	elem = begin->next;
	while (elem && elem != begin)
	{
		if (!ft_strcmp(name, elem->data->name))
			return (elem);
		elem = elem->next;
	}
	return (NULL);
}
