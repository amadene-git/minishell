#include "minishell.h"

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

t_dlist	*dlist_create_from_tab(char **tab)
{
	int		i;
	t_dlist *begin;
	t_dlist *elem1;
	t_dlist	*elem2;

	if (!tab[0] || !tab)
		return (NULL);
	begin = dlist_create_elem((void *)tab[0]);
	elem1 = begin;
	i = 1;
	while (tab[i])
	{
		if (!(elem2 = dlist_create_elem((void*)tab[i++])))
			return (NULL);
		elem2->prev = elem1;
		elem1->next = elem2;
		if (tab[i])
		{
			if (!(elem1 = dlist_create_elem((void*)tab[i++])))
				return (NULL);
			elem1->prev = elem2;
			elem2->next = elem1;
		}
	}
	if (!(i % 2))
	{
		begin->prev = elem2;
		elem2->next = begin;
	}
	else
	{
		begin->prev = elem1;
		elem1->next = begin;
	}
	return (begin);
}

void	dlist_print(t_dlist *begin, int fd)
{
	t_dlist	*elem;
	
	if (!begin)
		return;
	elem = begin;
	ft_putendl_fd(elem->data, fd);
	elem = elem->next;
	while (elem && elem != begin)
	{
			ft_putendl_fd(elem->data, fd);
			elem = elem->next;
	}
}

char	**tab_copy(char **tab)
{
	char 	**copy = NULL;
	int		i;

	i = -1;
	while (tab[++i]);
	copy = (char **)malloc(sizeof(char*) * (i + 1));
	i = -1;
	while (tab[++i])
		copy[i] = strdup(tab[i]);
	copy[i] = NULL;
	return (copy);
}

t_dlist *dlist_strchr_first(t_dlist *begin)
{
	t_dlist	*elem;
	t_dlist *ret;

	if (!begin)
		return (NULL);
	elem = begin->next;
	ret = begin;
	while (elem != begin)
	{
		if (ft_strcmp((char*)ret->data, (char*)elem->data) >= 0)
			ret = elem;
		elem = elem->next;
	}
	return (ret);
}

t_dlist	*dlist_chr_alpha_next(t_dlist *begin)
{
	char	*cmp;
	t_dlist	*elem;
	t_dlist *ret;

	if(!begin)
		return (NULL);
	elem = begin->next;
	ret = begin;
	while (elem != begin && ret == begin)
	{
		if (ft_strcmp(begin->data, elem->data) < 0)
			ret = elem;
		elem = elem->next;
	}		
	while (elem != begin)
	{
		
		if (ft_strcmp(begin->data, elem->data) < 0 && ft_strcmp(elem->data, ret->data) < 0)
			ret = elem;
		elem = elem->next;
	}
	if (ret == begin)
		return (NULL);
	return (ret);
}

int		built_in_env(int ac, char **av, t_dlist *envlist, int fd)
{
	if (ac > 1)
		return (-1);
	dlist_print(envlist, fd);
}



