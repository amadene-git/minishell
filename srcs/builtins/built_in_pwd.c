#include "../minishell.h"

void	built_in_pwd(size_t size, int fd)
{
	char buf[size + 1];
	if (!getcwd(&buf[0], size))
		ft_putendl_fd(strerror(errno), 2);
	else
		ft_putendl_fd(&buf[0], fd);
}

int main()
{
	built_in_pwd(300, 1);
	return (0);
}

