#include "minishell.h"

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
	[' '] = CHR_WORD,
	[' '] = CHR_WORD
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
// 	if (g_token_rules[g_get_token[g_get_chr['a']]][g_get_chr['b']])
// 		printf("C'est un moooot !");

// 	while (g_token_rules[CURRENT_TOKEN][CURRENT_CHR] == 1)
// 	{
// 		// on process
// 	}
// 	// On est plus dans un bon context donc on enregistre le token dans une structure
// 	while (s[i] != ' ')
// 	{
// 		i++
// 	}
// 	// on malloc, on recup le mot, on split.
// 	return (0);
// }

typedef struct s_list
{
	void			*data;
	struct s_list	*next;
	struct s_list	*prev;
}				t_list;

typedef struct	s_command
{
	int		word_type;
	char	**command_av;
}				t_command;

char	**ft_tabdup(char **tab)
{
	int 	i;
	char	**copy;
	
	if(!tab || !*tab)
		return(NULL);
	i = 0;
	while (tab[i])
		i++;
	if (!(copy = (char**)malloc(sizeof(char**) *(i + 1))))
		return (NULL);
	i = 0;
	while (tab[i])
	{
		copy[i] = tab[i];
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

char	**ft_push_str_back(char **tab, char *str)
{
	int 	i;
	char	**copy;

	if (!tab)
	{
		if(!(tab = (char**)malloc(sizeof(char**) * 2)))
			return (NULL);
		tab[0] = str;
		tab[1] = NULL;
		return (tab);
	}
	i = 0;
	while (tab[i])
		i++;
	if(!(copy = (char**)malloc(sizeof(char**) * (i + 2))))
		return (NULL);
	i = 0;
	while (tab[i])
	{
		copy[i] = tab[i];
		i++;
	}
	copy[i] = str;
	copy[i + 1] = NULL;
	free(tab);
	return (copy);
}


int main(int ac, char **av)
{
    ft_putstr_fd("salut\n", 1);
    return (0);
}


