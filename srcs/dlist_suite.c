/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dlist_suite.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:46:01 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/16 15:47:02 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_var	*create_var(const char *str)
{
	t_var	*variable;
	char	*ptr;

	ptr = ft_strchr(str, '=');
	variable = (t_var*)malloc(sizeof(t_var));
	if (ptr)
	{
		variable->name = ft_strndup(str, ptr - str);
		variable->value = ft_strdup(ptr + 1);
	}
	else
	{
		variable->name = ft_strdup(str);
		variable->value = NULL;
	}
	return (variable);
}

t_dlist	*insert_var(t_dlist *envlist, t_var *variable)
{
	t_dlist	*elem;

	if (!envlist || !variable)
		return (NULL);
	elem = dlist_create_elem(variable);
	elem->next = envlist;
	elem->prev = envlist->prev;
	elem->prev->next = elem;
	envlist->prev = elem;
	return (elem);
}

void	free_var(t_var *variable)
{
	if (variable->value)
		free(variable->value);
	if (variable->name)
		free(variable->name);
	free(variable);
}

void	free_elem(t_dlist *envlist, const char *name)
{
	t_dlist *elem;
	t_dlist	*rm;

	if (!(elem = dlist_chr(envlist, name)))
		return ;
	rm = elem;
	elem->prev->next = rm->next;
	elem->next->prev = rm->prev;
	free_var(rm->data);
	free(rm);
}

void	free_envlist(t_dlist *envlist)
{
	t_dlist	*elem;

	elem = envlist->next;
	while (elem != envlist)
	{
		elem = elem->next;
		free_var(elem->prev->data);
		free(elem->prev);
	}
	free_var(envlist->data);
	free(envlist);
}
