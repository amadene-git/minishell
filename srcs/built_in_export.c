#include "../includes/minishell.h"

void	print_value(char *str, int fd)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' || str[i] == '$' || str[i] == '"')
			ft_putchar_fd('\\', fd);
		ft_putchar_fd(str[i], fd);
		i++;
	}
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
			print_value(elem->data->value, fd);
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
				print_value(elem->data->value, fd);
				ft_putendl_fd("\"", fd);
			}
			else
				ft_putchar_fd('\n', fd);
		}
	}
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