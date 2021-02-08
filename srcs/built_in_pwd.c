#include "../includes/minishell.h"

int		built_in_pwd(int ac, char **av, t_dlist *envlist, int fd)
{
	char	buf[3200];

	if (ac != 1)
		return (-1);
	if (!getcwd(&buf[0], 31999))
	{
		ft_putendl_fd(strerror(errno), 2);
		return (-1);
	}	
	ft_putendl_fd(&buf[0], fd);
	return (0);
}

