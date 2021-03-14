/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 20:55:54 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/14 22:29:34 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int		print_export(t_dlist *envlist, int fd)
{
	t_dlist	*elem;

	elem = dlist_strchr_first(envlist);
	while (elem)
	{
		if (elem && ft_strcmp("_", elem->data->name)\
			&& ft_strcmp("?", elem->data->name))
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
		elem = dlist_chr_alpha_next(elem);
	}
	return (0);
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

int		export_error(t_var *var)
{
	ft_dprintf(2, "minishell: export: `%s", var->name);
	if (var->value)
		ft_dprintf(2, "=%s': not a valid identifier\n", var->value);
	else
		ft_dprintf(2, "': not a valid identifier\n");
	return (1);
}

int		built_in_export(int ac, char **av, t_dlist *envlist, int fd)
{
	int		i;
	int		status;
	t_dlist	*elem;
	t_var	*var;

	status = 0;
	if (ac == 1)
		return (print_export(envlist, fd));
	if (ac > 1 && !(i = 0))
		while (++i < ac)
		{
			var = create_var(av[i]);
			if (!is_valid_name(var->name))
				status = export_error(var);
			else if (!(elem = dlist_chr(envlist, var->name)))
				insert_var(envlist, var);
			else if (var->value)
			{
				free_var(elem->data);
				elem->data = var;
			}
			else
				free_var(var);
		}
	return (status);
}
