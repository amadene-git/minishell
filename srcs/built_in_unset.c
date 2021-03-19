/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 23:12:41 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/16 23:24:28 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		built_in_unset(t_cmd *cmd)
{
	int i;
	int	status;

	i = 0;
	status = 0;
	while (++i < cmd->ac)
	{
		if (!is_valid_name(cmd->av[i]))
		{
			ft_dprintf(2, "minishell: unset: `%s': not a valid identifier\n",
							cmd->av[i]);
			status = 1;
		}
		else if (!dlist_chr(cmd->envlist, cmd->av[i]))
			status = 1;
		else
			free_elem(cmd->envlist, cmd->av[i]);
	}
	return (status);
}

char	**get_env_from_envlist(t_dlist *begin, t_dlist *elem, int lvl)
{
	char	**env;

	if (elem->next != begin)
	{
		if (elem->data->value && ft_strcmp(elem->data->name, "?"))
			env = get_env_from_envlist(begin, elem->next, lvl + 1);
		else
			env = get_env_from_envlist(begin, elem->next, lvl);
	}
	else if (ft_strcmp(elem->data->name, "?"))
	{
		env = (char**)malloc(sizeof(char*) * (lvl + 2));
		env[lvl + 1] = NULL;
		env[lvl] = get_var_str(elem->data);
		return (env);
	}
	else
	{
		env = (char**)malloc(sizeof(char*) * (lvl + 1));
		env[lvl] = NULL;
		return (env);
	}
	if (elem->data->value && ft_strcmp(elem->data->name, "?"))
		env[lvl] = get_var_str(elem->data);
	return (env);
}

void	envlist_print(const char **env, int fd)
{
	int	i;

	i = -1;
	while (env[++i])
		ft_putendl_fd(env[i], fd);
}

int		built_in_env(t_cmd *cmd)
{
	if (cmd->ac != 1)
		return (-1);
	envlist_print((const char**)cmd->env, cmd->fdout);
	return (0);
}

char	*get_var_str(t_var *var)
{
	char	*str;
	char	*value;
	int		i;

	if (var && var->name)
		str = ft_strjoindoublefree(ft_strdup(var->name), ft_strdup("="));
	if (var->value)
		value = ft_strdup(var->value);
	else
		value = NULL;
	i = -1;
	while (value && value[++i])
	{
		if (value[i] == '"' || value[i] == '\\' || value[i] == '$')
		{
			value = insert_string(value, ft_strdup("\\"), i, i);
			i++;
		}
	}
	if (str && value)
		str = ft_strjoindoublefree(str, value);
	return (str);
}
