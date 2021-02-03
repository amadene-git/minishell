#include "../includes/minishell.h"

void	free_array(char **array)
{
	for (int i = 0; array[i]; i++) {
		free(array[i]);
		array[i] = NULL;
	}
	free(array);
	array = NULL;
}

void	get_absolute_path(char **cmd)
{
	char	*path = ft_strdup(getenv("PATH"));
	char	*bin = NULL;
	char	**path_split = NULL;
	char	*str = ft_strdup(cmd[0]);

	if (path == NULL) // si le path est null, on cree un path
		path = ft_strdup("/bin:/usr/local/bin:/usr/bin:/bin:/usr/local/sbin");

	// si cmd n'est pas le chemin absolue, on cherche le chemin absolue du
	// binaire grace a la variable d'environment PATH
	if (cmd[0][0] != '/' && strncmp(cmd[0], "./", 2) != 0) {

		// On split le path pour verifier ou ce trouve le binaire
		path_split = ft_split(path, ':');
		free(path);
		path = NULL;

		// On boucle sur chaque dossier du path pour trouver l'emplacement du binaire
		for (int i = 0; path_split[i]; i++) {
			// alloc len du path + '/' + len du binaire + 1 pour le '\0'
			bin = (char *)calloc(sizeof(char), (strlen(path_split[i]) + 1 + strlen(cmd[0]) + 1));
			if (bin == NULL)
				break ;

			// On concat le path , le '/' et le nom du binaire
			strcat(bin, path_split[i]);
			strcat(bin, "/");
			strcat(bin, cmd[0]);

			// On verfie l'existence du fichier et on quitte la boucle si access
			// renvoi 0
			if (access(bin, F_OK) == 0)
				break ;

			// Nous sommes des gens propre :D
			free(bin);
			bin = NULL;
		}
		free_array(path_split);

		// On remplace le binaire par le path absolue ou NULL si le binaire
		// n'existe pas
		free(cmd[0]);
		cmd[0] = (bin) ? bin : ft_strdup(str);
	} else {
		free(path);
		path = NULL;
	}
	free(str);
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

int	exec_bin(char **cmd, char **env, int flag, int fd[2])
{
	pid_t	pid = 0;
	int		status = 0;

	pid = fork();
	if (pid == -1)
		ft_putstr_fd(strerror(errno), 2);
	else if (pid == 0)
	{
		get_absolute_path(cmd);
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
