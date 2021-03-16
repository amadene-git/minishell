/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 14:41:36 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/16 15:39:07 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_if_xprivilege(t_cmd *cmd)
{
	int	fd;

	fd = open(cmd->bin, O_RDONLY);
	if (fd == -1)
	{
		ft_dprintf(2, "minishell: %s: Permission denied\n", cmd->bin);
		return (126);
	}
	else
		close(fd);
	if (execve(cmd->bin, cmd->av, cmd->env) == -1)
	{
		if (errno == 8)
			return (0);
		if (errno == 13)
		{
			ft_dprintf(2, "minishell: %s: command not found\n", cmd->bin);
			return (127);
		}
		return (126);
	}
	return (0);
}

static int	handle_reg_dir(t_cmd *cmd, struct stat buff)
{
	if (S_ISDIR(buff.st_mode) && ((cmd->av[0][0] == '.' && cmd->av[0][1]
				== '/') || cmd->av[0][ft_strlen(cmd->av[0]) - 1] == '/'))
	{
		ft_dprintf(2, "minishell: %s: is a directory\n", cmd->bin);
		return (126);
	}
	else if (buff.st_mode & S_IXUSR)
		return (handle_if_xprivilege(cmd));
	else if (ft_strchr(cmd->bin, '/') == NULL)
	{
		ft_dprintf(2, "minishell: %s: command not found\n", cmd->bin);
		return (127);
	}
	ft_dprintf(2, "minishell: %s: Permission denied\n", cmd->bin);
	return (126);
}

int			exec_bin(t_cmd *cmd)
{
	struct stat buff;

	if (lstat(cmd->bin, &buff) != -1)
	{
		if (S_ISLNK(buff.st_mode))
			stat(cmd->bin, &buff);
		if (access(cmd->bin, F_OK))
		{
			ft_dprintf(2, "minishell: %s: No such file or directory\n",\
						cmd->bin);
			return (127);
		}
		else if (buff.st_mode & S_IFREG || S_ISDIR(buff.st_mode))
			return (handle_reg_dir(cmd, buff));
	}
	else
	{
		ft_dprintf(2, "minishell: %s: command not found\n", cmd->bin);
		return (127);
	}
	return (0);
}

int			exec_no_fork(t_cmd *cmd)
{
	int	status;

	status = 0;
	if (!ft_strcmp("exit", cmd->bin))
	{
		status = exit_cmd(cmd);
	}
	else if (!ft_strcmp("cd", cmd->bin))
	{
		status = built_in_cd(cmd->ac, cmd->av, cmd->envlist);
	}
	else if (!ft_strcmp("unset", cmd->bin))
	{
		status = built_in_unset(cmd);
	}
	else if (!ft_strcmp("export", cmd->bin) && cmd->ac > 1)
	{
		status = built_in_export(cmd->ac, cmd->av, cmd->envlist, 1);
	}
	return (status);
}
