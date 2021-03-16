/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 14:47:55 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/16 15:57:44 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		is_builtin(char *cmd)
{
	if (!ft_strcmp("echo", cmd) ||\
			!ft_strcmp("pwd", cmd) ||\
			!ft_strcmp("cd", cmd) ||\
			!ft_strcmp("env", cmd) ||\
			!ft_strcmp("export", cmd) ||\
			!ft_strcmp("unset", cmd))
		return (1);
	return (0);
}

int		exec_built_in(t_cmd *cmd)
{
	int status;

	status = 0;
	if (!ft_strcmp("echo", cmd->bin))
	{
		built_in_echo(cmd->ac, cmd->av, 1);
	}
	else if (!ft_strcmp("pwd", cmd->bin))
	{
		status = built_in_pwd(cmd->ac, cmd->av);
	}
	else if (!ft_strcmp("env", cmd->bin))
	{
		status = built_in_env(cmd);
	}
	else if (!ft_strcmp("export", cmd->bin) && cmd->ac == 1)
	{
		status = built_in_export(cmd->ac, cmd->av, cmd->envlist, 1);
	}
	return (status);
}
