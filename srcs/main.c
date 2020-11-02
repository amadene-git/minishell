#include "../includes/minishell.h"

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);

}

#include <stdio.h>
enum e_token{
 TOK_ERROR,
 TOK_WORD,
 TOK_OP,
 TOK_SP
};
enum e_chr{
 CHR_ERROR,
 CHR_WORD,
 CHR_OP,
 CHR_SP
};
unsigned int g_get_chr[255] = {
 ['A'...'Z'] = CHR_WORD,
 ['a'...'z'] = CHR_WORD,
 ['0'...'9'] = CHR_WORD,
 ['&'] = CHR_OP,
 ['|'] = CHR_OP,
 ['>'] = CHR_OP,
 ['<'] = CHR_OP,
 [' '] = CHR_SP,
 [' '] = CHR_SP
};
unsigned int g_get_token[CHR_SP + 1] = {
 [CHR_WORD] = TOK_WORD,
 [CHR_OP] = TOK_OP,
 [CHR_SP] = TOK_SP
};
unsigned int g_token_rules[TOK_SP + 1][CHR_SP + 1] = {
 [TOK_WORD] = {
  [CHR_WORD] = 1
 },
 [TOK_SP] = {
  [CHR_SP] = 1,
 }
};

// int main(int ac, char **av)
// {
//  if (g_token_rules[g_get_token[g_get_chr['a']]][g_get_chr['b']])
//   printf("C'est un moooot !");
	
//  while (g_token_rules[CURRENT_TOKEN][CURRENT_CHR] == 1)
//  {
//   // on process
//  }
//  // On est plus dans un bon context donc on enregistre le token dans une structure
//  while (s[i] != ' ')
//  {
//   i++
//  }
//  // on malloc, on recup le mot, on split.
//  return (0);
// }


char	*ft_strdup(char *str)
{
	char 	*dup;
	int		i;

	if (str == NULL)
		return (calloc(1,1));
	if(!(dup = (char*)malloc(ft_strlen(str) + 1)))
		return (NULL);
	i = 0;
	while (str[i])
		dup[i] = str[i++];
	dup[i] = 0;
	return (dup);	

}

char *ft_strjoinfree(char *s1, char *s2)
{
	int		size;
	char	*str;
	int		i;

	if (s1 == NULL || s2 == NULL)
		return ((s1 == NULL) ? s2 : s1);
	if (!*s1)
	{
		free(s1);
		return (s2);
	}
	if (!*s2)
	{
		free(s2);
		return (s1);

	}
	size = ft_strlen(s1) + ft_strlen(s2);
	if(!(str = malloc(sizeof(char) * (size + 1))))
		return (NULL);
	i = 0;
	while (*s1)
		str[i++] = *(s1++);
	free(s1 - i);
	size = size - i;
	while (*s2)
		str[i++] = *(s2++);
	free(s2 - size);
	str[i] = 0;
	return (str);
}

char	*put_char_end_str(char *str, char c)
{
	char	*copy;
	int		i;

	i = 0;
	copy = (char*)malloc(sizeof(char) * (ft_strlen(str) + 2));
	while (str[i])
	{
		copy[i] = str[i];
		i++;
	}
	copy[i] = c;
	i++;
	copy[i] = 0;
	free(str);
	return (copy);
}

int		index_command(char *str)
{
	if (!strcmp(str, "exit"))
		return (0);
	if (!strcmp(str, "echo"))
		return (1);
	if (!strcmp(str, "cd"))
		return (2);
	if (!strcmp(str, "pwd"))
		return (3);
	if (!strcmp(str, "export"))
		return (4);
	if (!strcmp(str, "unset"))
		return (5);
	if (!strcmp(str, "env"))
		return (6);
	if (!strcmp(str, ";"))
		return (7);
	if (!strcmp(str, "<"))
		return (8);
	if (!strcmp(str, ">"))
		return (9);
	if (!strcmp(str, ">>"))
		return (10);
	if (!strcmp(str, "|"))
		return (11);
	return (-1);
}

char	**ft_copy_tab(char **tab, int start, int end)
{
	char **copy;
	int i;

	if (start > end)
		return (NULL);
	copy = (char**)malloc(sizeof(char**) * (end - start + 1));
	i = 0;
	while (start <= end && tab[start])
	{
		copy[i] = tab[start];
		start++;
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

int		exec_script(char **tab)
{
	int i;

	i = 0;
	while (tab[i])
	{
		if (index_command(tab[i]) == -1)
		{
			printf("error \n");
			return 0;
		}
		
		i++;
	}
	return (1);

}

char ft_getchar()
{
	char c;

	read(0, &c, 1);
	return (c);
}

char *ft_read_stdin()
{
	char *str;
	char c;
	
	c = 0;
	str = calloc(1,1);
	while(c != '\n') 
	{
		c = ft_getchar();
		str = put_char_end_str(str, c);
	}
	return (str);
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

#include <fcntl.h>


int main(int ac, char **av, char ** envp)
{
	int fd = open("test", O_CREAT | O_RDWR);
	int pipefd[2];
	pipefd[0] = 1;
	pipefd[1] = fd;
	write(1, "abc", 3);
	pipe(pipefd);
	close(fd);
	return (0);
}

