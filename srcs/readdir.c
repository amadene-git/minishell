#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int main()
{
	DIR				*dir;
	struct dirent	*sd;

	if (!(dir = opendir(".")))
	{
		return (-1);
	}
	while ((sd = readdir(dir)))
	{
		printf("->%s\n", sd->d_name);
	}
	closedir(dir);

	return (0);

}
