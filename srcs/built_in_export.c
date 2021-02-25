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
	if (elem && ft_strcmp("_", elem->data->name))
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
		if (elem && ft_strcmp("_", elem->data->name))
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

int		is_valid_name(char *str)
{
	int i;

	i = 0;
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		if (ft_isalpha(*str) || *str == '_')
			i = 1;
		str++;
	}
	return (i);

}

char	*ft_str_trim_backslash(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\\')
		{
			str = insert_string(ft_strdup(str), ft_strdup(""), i, i + 1);
			i++;
		}
		if (str[i])
			i++;
	}
	return (str);
}

int		built_in_export(int ac, char **av, t_dlist *envlist, int fd)
{
	int		i;
	int		status;
	t_dlist	*elem;
	t_var	*var;

	status = 0;
	if (ac == 1)
	{
		print_export(envlist, fd);		
		return (status);
	}
	i = 0;
	if (ac > 1)
	{
		while (++i < ac)
		{
			var = create_var(av[i]);
			if (!is_valid_name(var->name))
			{
				dprintf(2, "minishell: export: `%s",var->name);
				if (var->value)
					dprintf(2, "=%s': not a valid identifier\n", var->value);
				else	
					dprintf(2, "': not a valid identifier\n");
				status = 1;
			}
			else if (!(elem = dlist_chr(envlist, var->name)) && !(status = 0))
				insert_var(envlist, var);
			else if (var->value && !(status = 0))
			{
				free_var(elem->data);
				elem->data = var;
			}
			else if (!(status = 0))
				free_var(var);
		}
	}
	return (status);
}
