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

void	exec_built_in(int ac, char **cmd, t_dlist *envlist, int fd)
{
	
	if (!ft_strcmp("echo", cmd[0]))
	{
		built_in_echo(ac, cmd, envlist, fd);
	}
	else if (!ft_strcmp("cd", cmd[0]))
	{
		built_in_cd(ac, cmd, envlist, fd);
	}
	else if (!ft_strcmp("pwd", cmd[0]))
	{
		built_in_pwd(ac, cmd, envlist, fd);
	}
	else if (!ft_strcmp("env", cmd[0]))
	{
		built_in_env(ac , cmd, envlist, fd);
	}
	else if (!ft_strcmp("export", cmd[0]))
	{
		built_in_export(ac, cmd, envlist, fd);
	}
	else if (!ft_strcmp("unset", cmd[0]))
	{
		built_in_unset(ac, cmd, envlist, fd);
	}
}

int	exec_bin(char **cmd, char **env, int flag, int fd[2], t_dlist *envlist)
{
	pid_t	pid = 0;
	int		status = 0;

	pid = fork();
	if (pid == -1)
		ft_putstr_fd(strerror(errno), 2);
	else if (pid == 0)
	{
		get_absolute_path(cmd, envlist);
		if (flag > 0)
		{
			if (flag == 1)
				dup2(fd[1], STDOUT_FILENO);
			else if (flag == 2)
				dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
			close(fd[1]);
		}
		if (execve(cmd[0], cmd, env) == -1)
		{
			dprintf(2, "minishell: %s: command not found\n", cmd[0]);
			return (-1);
		}
	}
	if (flag == 2)
	{
		close(fd[0]);
		close(fd[1]);
	}
	waitpid(pid, NULL, 0);
	//kill(pid, SIGTERM);
	return (0);
}
