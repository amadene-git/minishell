#include "../includes/minishell.h"

t_dlist *dlist_create_elem(void *data)
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
	printf("%s->%p\tprev->%p\tnext->%p\n", (char *)elem->data, elem, elem->prev, elem->next);
}

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

t_dlist *dlist_strchr_first(t_dlist *begin)
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

	if(!begin)
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
		
		if (ft_strcmp(begin->data->name, elem->data->name) < 0 && ft_strcmp(elem->data->name, ret->data->name) < 0)
			ret = elem;
		elem = elem->next;
	}
	if (ret == begin)
		return (NULL);
	return (ret);
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

void	free_var(t_var *variable)
{
	if (variable->value)
		free(variable->value);
	free(variable->name);
	free(variable);
}

void	free_elem(t_dlist *envlist, const char *name)
{
	t_dlist *elem;
	t_dlist	*rm;

	if (!(elem = dlist_chr(envlist, name)))
		return;
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



