#include "get_next_line.h"

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

void    ft_exec(char *line)
{
    system(line);
}

int main(int ac, char **av, char ** envp)
{
    char *line;

    while (1)
    {
        write(1, "$>", 2);
        get_next_line(0, &line);
        //ft_parse(line);
        ft_exec(line);
    }
	return (0);
}

