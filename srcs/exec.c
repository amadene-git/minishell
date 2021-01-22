#include "minishell.h"

char	**split(char *raw_cmd, char *limit)
{
	char	*ptr = NULL;
	char	**cmd = NULL;
	size_t	idx = 0;

	// split sur les espaces
	ptr = strtok(raw_cmd, limit);

	while (ptr) {
		cmd = (char **)realloc(cmd, ((idx + 1) * sizeof(char *)));
		cmd[idx] = strdup(ptr);
		ptr = strtok(NULL, limit);
		++idx;
	}
	// On alloue un element qu'on met a NULL a la fin du tableau
	cmd = (char **)realloc(cmd, ((idx + 1) * sizeof(char *)));
	cmd[idx] = NULL;
	return (cmd);
}

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
	char	*path = strdup(getenv("PATH"));
	char	*bin = NULL;
	char	**path_split = NULL;

	if (path == NULL) // si le path est null, on cree un path
		path = strdup("/bin:/usr/local/bin:/usr/bin:/bin:/usr/local/sbin");

	// si cmd n'est pas le chemin absolue, on cherche le chemin absolue du
	// binaire grace a la variable d'environment PATH
	if (cmd[0][0] != '/' && strncmp(cmd[0], "./", 2) != 0) {

		// On split le path pour verifier ou ce trouve le binaire
		path_split = split(path, ":");
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
		cmd[0] = bin;
	} else {
		free(path);
		path = NULL;
	}
}

int		is_builtin(char	*cmd)
{
	if (!ft_strcmp("echo", cmd) ||\
			!ft_strcmp("pwd", cmd) ||\
			!ft_strcmp("cd", cmd) ||\
			!ft_strcmp("env", cmd))
			//!ft_strcmp("export", cmd))
			//!ft_strcmp("unset", cmd))
			//!ft_strcmp("exit", cmd))
		return (1);
	return (0);
}

void	exec_built_in(int ac, char **cmd, t_dlist *envlist, int fd)
{
	
	if (!ft_strcmp("echo", cmd[0]))
	{
		printf ("echo:\n");
		built_in_echo(ac, cmd, envlist, fd);
	}
	else if (!ft_strcmp("cd", cmd[0]))
	{
		printf ("cd:\n");
		built_in_cd(ac, cmd, envlist, fd);
	}
	else if (!ft_strcmp("pwd", cmd[0]))
	{
		printf("pwd:\n");
		built_in_pwd(ac, cmd, envlist, fd);
	}
	else if (!ft_strcmp("env", cmd[0]))
	{
		printf("env:\n");
		built_in_env(ac , cmd, envlist, fd);
	}
/*	else if (!ft_strcmp("export", cmd[0]))
	{
		built_in_export(env);
	}*/
}

void	exec_cmd(char **cmd, char **env)
{
	pid_t	pid = 0;
	int		status = 0;

	pid = fork();
	if (pid == -1)
		ft_putstr_fd(strerror(errno), 2);
	else if (pid > 0) 
	{
		waitpid(pid, &status, 0);
		kill(pid, SIGTERM);
	}
	else 
	{
		ft_putstr_fd("UNIX->\n", 1);
		get_absolute_path(cmd);
		if (execve(cmd[0], cmd, env) == -1)
			ft_putstr_fd(strerror(errno), 2);
		exit(EXIT_FAILURE);
	}
}

