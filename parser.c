#include <stdlib.h>
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

t_list	*ft_create_elem(void *data)
{
	t_list	*elem;

	if(!(elem = (t_list*)malloc(sizeof(t_list))))
		return (NULL);
	elem->next = NULL;
	elem->prev = NULL;
	elem->data = data;
	return (elem);
}

int		ft_push_data_back(t_list **begin_list, void *data)
{
	t_list	*current;
	t_list	*elem;

	printf("1\n");
	if (!begin_list)
		return (-1);
	if(!*begin_list)
	{
		if(!(*begin_list = ft_create_elem(data)))
			return (-1);
		return (0);
	}
	current = *begin_list;
	if(!(elem = ft_create_elem(data)))
		return (-1);
	printf("2\n");
	while (current->next)
		current = current->next;
	current->next =	elem;
	elem->prev = current;
	printf("3\n");
	return (0);
}

int		ft_push_data_front(t_list **begin_list, void *data)
{
	t_list	*swp;

	if (!begin_list)
		return (-1);
	if (!*begin_list)
	{
		if(!(*begin_list = ft_create_elem(data)))
			return (-1);
		return (0);
	}
	swp = *begin_list;
	if(!(*begin_list = ft_create_elem(data)))
		return (-1);
	(*begin_list)->next = swp;
	swp->prev = (*begin_list);
	return (0);
}

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


char	*ft_strndup(char *str, int n)
{
	char	*dup;
	int		i;
	
	if(!(dup = (char*)calloc(1, n + 1)))
		return (NULL);
	i = 0;
	while (str[i] && i < n)
	{
		dup[i] = str[i];
		i++;
	}
	return (dup);
}

t_list	**create_list(const char *str)
{
	int			i;
	t_command	*block;

	i = 0;
	while (*str)
	{
		while (*str && !g_token_rules[g_get_token[g_get_chr[';']]][g_get_chr[*str])
		{
			while (g_token_rules[g_get_token[g_get_chr['a']]][g_get_chr[str[i]]])
				i++;
			tab = ft_push_str_back(tab, ft_strndup(str, i - 1));
			str += i;
			i = 0;
			while (g_token_rules[g_get_token[g_get_chr[' ']]][g_get_chr[*str])
				str++;
		}
		while (g_token_rules[g_get_token[g_get_chr[';']]][g_get_chr[str[i]]])
	}
	return (NULL);
}

int main(int ac, char **av)
{
	t_list **begin_list = (t_list**)malloc(sizeof(t_list**));
	int i = 0;

	char *str = "lolilol";
	char **tab = ft_push_str_back(av, str);
	while (tab[i])
		printf("%s\n", tab[i++]);
}


