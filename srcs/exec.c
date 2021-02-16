#include "../includes/minishell.h"

void	get_absolute_path(char **cmd, t_dlist *envlist)
{
	char			*path = NULL;
	char			*strdir = NULL;
	DIR				*dir;
	struct dirent	*sd;

	if (dlist_chr(envlist, "PATH"))
		path = dlist_chr(envlist, "PATH")->data->value;
	else
		return;
	if (cmd[0][0] != '/' && strncmp(cmd[0], "./", 2) != 0)
		while (path)
		{
			if (ft_strchr(path, ':'))
			{
				strdir = ft_strndup(path, ft_strchr(path, ':') - path);
				path = ft_strchr(path, ':') + 1;
			}
			else
			{
				strdir = ft_strdup(path);
				path = NULL;
			}
			if ((dir = opendir(strdir)) == NULL)
				continue;
			while ((sd = readdir(dir)) != NULL)
				if (!ft_strcmp(sd->d_name, cmd[0]))
				{
					free(cmd[0]);
					strdir = ft_strjoindoublefree(strdir, ft_strdup("/"));
					cmd[0] = ft_strjoindoublefree(strdir, ft_strdup(sd->d_name));
					return;
				}
			closedir(dir);
			free(strdir);
		}
}

int		is_builtin(char	*cmd)
{
	if (!ft_strcmp("echo", cmd) ||\
			!ft_strcmp("pwd", cmd) ||\
			!ft_strcmp("cd", cmd) ||\
			!ft_strcmp("env", cmd) ||\
			!ft_strcmp("export", cmd) ||\
			!ft_strcmp("unset", cmd))
			//!ft_strcmp("exit", cmd))
		return (1);
	return (0);
}

void	exec_built_in(t_cmd *cmd)
{
	if (!ft_strcmp("echo", cmd->bin))
	{
		built_in_echo(cmd->ac, cmd->av, cmd->envlist, 1);
	}
	else if (!ft_strcmp("cd", cmd->bin))
	{
		built_in_cd(cmd->ac, cmd->av, cmd->envlist, 1);
	}
	else if (!ft_strcmp("pwd", cmd->bin))
	{
		built_in_pwd(cmd->ac, cmd->av, cmd->envlist, 1);
	}
	else if (!ft_strcmp("env", cmd->bin))
	{
		built_in_env(cmd);
	}
	else if (!ft_strcmp("export", cmd->bin))
	{
		built_in_export(cmd->ac, cmd->av, cmd->envlist, 1);
	}
	else if (!ft_strcmp("unset", cmd->bin))
	{
		built_in_unset(cmd);
	}
}

int	exec_bin(t_cmd *cmd)
{
	get_absolute_path(cmd->av, cmd->envlist);
	if (execve(cmd->av[0], cmd->av, cmd->env) == -1)
	{
		dprintf(2, "minishell: %s: command not found\n", cmd->bin);
		return (-1);
	}
	return (0);
}
