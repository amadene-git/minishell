#include "../includes/minishell.h"

unsigned int g_get_chr[255] = {
	['!'...'%'] = CHR_WORD,
	['*'...':'] = CHR_WORD,
	['?'...'{'] = CHR_WORD,
	['}'...'~']	= CHR_WORD,
	['"'] = CHR_STR,
	['\''] = CHR_ST,
	['\t'] = CHR_SP,
	['\n'] = CHR_SP,
	[' '] = CHR_SP,
	['&'] = CHR_OP,
	['|'] = CHR_OP,
	['>'] = CHR_OP,
	['<'] = CHR_OP,
	[';'] = CHR_OP,
	['\0'] = CHR_END
};

t_tok	**lexer(char *str, int *i, int lvl)
{
	t_tok	**tab;
	char	*value;
	int		type;
	int		j;

//	printf ("*******lexer str:%s i:%d lvl:%d*******\n", str + *i, *i, lvl);
	
//	while (g_token_rules[g_get_token[g_get_chr[' ']]][g_get_chr[str[*i]]])
//		(*i)++;
	j = *i;
	value = NULL;
	type = 6;
	if (g_get_chr[str[*i]] == CHR_STR)
	{
		while (g_get_chr[str[++j]] != CHR_STR && str[j])
			if (str[j] == '\\' && str[j + 1])
				j++;
		if (str[j])
			j++;
		else
			type = CHR_ERROR;
	}
	else if (g_get_chr[str[*i]] == CHR_ST)
	{
		while (g_get_chr[str[++j]] != CHR_ST && str[j]);
		if (str[j])
			j++;
		else
			type = CHR_ERROR;
		
	}
	else
		while (g_get_chr[str[*i]] == g_get_chr[str[j]])
		{
			if (str[j]== '\\' && str[j + 1])
				j++;
			if (str[j])
				j++;
			else
				type = CHR_ERROR;
		}
	if (g_get_chr[str[*i]] != CHR_SP)
		value = ft_strndup(str + *i, j - *i);
	else
		value = NULL;
	if (type != CHR_ERROR)
		type = g_get_chr[str[*i]];
	*i = j;
	if (g_get_chr[str[j]] != CHR_END)
		tab = lexer(str, i, lvl + 1);
	else
	{
		tab = (t_tok**)malloc(sizeof(t_tok*) * (lvl + 2));
		tab[lvl + 1] = (t_tok*)malloc(sizeof(t_tok));
		tab[lvl + 1]->value = NULL;
		tab[lvl + 1]->type = CHR_END;
	}	
	tab[lvl] = (t_tok*)malloc(sizeof(t_tok));
	tab[lvl]->value = value;
	tab[lvl]->type = type;
		
	return (tab);
}

char	*put_var_env(char *str, t_dlist *envlist)
{
	int		i;
	int 	j;
	char	*name;
	char	*var;
	t_dlist *elem;

	i = 0;
	while (str[i])
	{
	//	printf("1\n");
		j = i;
		if (str[i] == '\\' && str[i + 1])
			i += 2;
		else if (str[j++] == '$')
		{
			while (ft_isalpha(str[j]))
				j++;
	//		printf("2\n");
			name = ft_strndup(str + i + 1, j - i - 1);
			if (dlist_chr(envlist, name))
				str = insert_string(str, ft_strdup(dlist_chr(envlist, name)->data->value), i, j );
			else
				str = insert_string(str, ft_strdup(""), i, j );
			free(name);
		}
		else
			i++;
	}
	return (str);

}

char	*get_st(const char *str)
{
	int	i;

	if (*str != '\'')
		return (NULL);
	i = 0;
	while (str[++i]);
	return (ft_strndup(str + 1, i - 2));
}

char	*get_str(const char *str)
{
	int	i;

	if (*str != '"')
		return (NULL);
	i = 0;
	while (str[++i]);
	return (ft_strndup(str + 1, i - 2));
}

t_tok	**get_cmd(t_tok **tok_lex,  t_cmd *cmd, int lvl)
{
	int		i;
	char	*str;
	char	*s1 = NULL;
	char	*s2 = NULL;

	i = 0;
	if (tok_lex[i]->type >= CHR_WORD && tok_lex[i]->type <= CHR_SP)
	{
		while (tok_lex[i]->type == CHR_SP)
			i++;
		while (tok_lex[i]->type >= CHR_WORD && tok_lex[i]->type <= CHR_ST)
		{
			if (tok_lex[i]->type == CHR_ST)
			{
				s2 = get_st(tok_lex[i]->value);
			}
			else if (tok_lex[i]->type == CHR_STR)
			{
				s2 = get_str(tok_lex[i]->value);
				s2 = put_var_env(s2, cmd->envlist);
			}
			else if (tok_lex[i]->type == CHR_WORD)
			{
				s2 = ft_strdup(tok_lex[i]->value);
				s2 = put_var_env(s2, cmd->envlist);
			}
			s1 = ft_strjoindoublefree(s1, s2);
			i++;
		}
		tok_lex = get_cmd(tok_lex + i, cmd, lvl + 1);
	}
	else
	{
		cmd->av = (char**)malloc(sizeof(char*) * (lvl + 1));
		cmd->av[lvl] = NULL;
		cmd->ac = (tok_lex[i - 1]->type == CHR_SP) ? lvl - 1 : lvl;
		//cmd->ac = lvl;
		return (tok_lex + i);
	}
	cmd->av[lvl] = s1;
	return (tok_lex);

}

