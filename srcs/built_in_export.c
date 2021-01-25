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

t_dlist	*dlist_create_from_tab(const char **tab)
{
	int		i;
	t_dlist *begin;
	t_dlist *elem1;
	t_dlist	*elem2;

	if (!tab[0] || !tab)
		return (NULL);
	begin = dlist_create_elem(create_var(tab[0]));
	elem1 = begin;
	i = 1;
	while (tab[i])
	{
		if (!(elem2 = dlist_create_elem(create_var(tab[i++]))))
			return (NULL);
		elem2->prev = elem1;
		elem1->next = elem2;
		if (tab[i])
		{
			if (!(elem1 = dlist_create_elem(create_var(tab[i++]))))
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
	ft_putstr_fd(elem->data->name, fd);
	if (elem->data->value)
	{
		ft_putchar_fd('=', fd);
		ft_putendl_fd(elem->data->value, fd);
	}
	else
		ft_putchar_fd('\n', fd);
	elem = elem->next;
	while (elem && elem != begin)
	{
			ft_putstr_fd(elem->data->name, fd);
			if (elem->data->value)
			{
				ft_putchar_fd('=', fd);
				ft_putendl_fd(elem->data->value, fd);
			}
			else
				ft_putchar_fd('\n', fd);
			elem = elem->next;
	}
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
	char	*cmp;
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

void	print_export(t_dlist *envlist, int fd)
{
	t_dlist	*elem;

	elem = dlist_strchr_first(envlist);
	if (elem)
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putstr_fd(elem->data->name, fd);
		if (elem->data->value)
		{
			ft_putstr_fd("=\"", fd);
			ft_putstr_fd(elem->data->value, fd);
			ft_putendl_fd("\"", fd);
		}
		else
			ft_putchar_fd('\n', fd);
	}
	while (elem)
	{
		elem = dlist_chr_alpha_next(elem);
		if (elem)
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putstr_fd(elem->data->name, fd);
			if (elem->data->value)
			{
				ft_putstr_fd("=\"", fd);
				ft_putstr_fd(elem->data->value, fd);
				ft_putendl_fd("\"", fd);
			}
			else
				ft_putchar_fd('\n', fd);
		}
	}
}

void	insert_var(t_dlist *envlist, t_var *variable)
{
	t_dlist	*elem;

	elem = dlist_create_elem(variable);
	elem->next = envlist;
	elem->prev = envlist->prev;
	elem->prev->next = elem;
	envlist->prev = elem;
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

int		built_in_export(int ac, char **av, t_dlist *envlist, int fd)
{
	int		i;
	t_dlist	*elem;
	t_var	*var;

	if (ac == 1)
	{
		print_export(envlist, fd);		
		return (0);
	}
	i = 0;
	if (ac > 1)
	{
		while (++i < ac)
		{
			var = create_var(av[i]);
			if (!(elem = dlist_chr(envlist, var->name)))
				insert_var(envlist, var);
			else if (var->value)
			{
				free_var(elem->data);
				elem->data = var;
			}
			else
				free_var(var);
		}
		return (0);
	}
	else
		return (-1);
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

int		built_in_unset(int ac, char **av, t_dlist *envlist, int fd)
{
	int i;

	i = 0;
	while (++i < ac)
		free_elem(envlist, av[i]);
	return (0);

}

int		built_in_env(int ac, char **av, t_dlist *envlist, int fd)
{
	if (ac != 1)
		return (-1);
	dlist_print(envlist, fd);
	return (0);
}



