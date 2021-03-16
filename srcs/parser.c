/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 14:34:58 by admadene          #+#    #+#             */
/*   Updated: 2021/03/16 14:35:06 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_st(char *str)
{
	int	i;

	if (*str != '\'')
		return (NULL);
	i = 0;
	while (str[++i]);
	return (ft_strndup(str + 1, i - 2));
}

char	*get_str(char *str, t_dlist *envlist)
{
	int		i;
	int		j;
	char	*ptr;
	t_dlist	*elem;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
    {
        if (str[i] == '\\')
        {
			if (str[i + 1] == '"' || str[i + 1] == '$' || str[i + 1] == '\\')
            	str = insert_string(str, strdup(""), i, i + 1);
        }
        else if (str[i] == '$')
        {
            j = i + 1;
            while (ft_isalpha(str[j]) || ft_isdigit(str[j]) || str[j] == '_')
                j++;
            ptr = ft_strndup(str + i + 1, j - (i + 1));
			if ((elem = dlist_chr(envlist, ptr)) && elem->data->value)
			{
            	str = insert_string(str, ft_strdup(elem->data->value), i, j);
				i += ft_strlen(elem->data->value) - 1;
			}
			else
			{
            	str = insert_string(str, strdup(""), i, j);
				i--;
			}
        }
		i++;
    }
	return (str);
}

char	*str_clean_whitespaces(const char *str)
{
	int 	i;
	char	**tab;
	char	*ret;

	tab = ft_split(str, ' ');
	i = 0;
	if (tab && tab[i])
		ret =ft_strdup(tab[i]);
	i = 1;
	while (tab[i])
	{
		ret = ft_strjoindoublefree(ret, ft_strdup(" "));
		ret = ft_strjoindoublefree(ret, tab[i]);
		i++;
	}
	return (ret);
}

char	*get_word(char *str, t_dlist *envlist)
{
	int		i;
	int 	j;
	char	*ptr;
	t_dlist	*elem;

	i = 0;
	while (str[i])
    {
        if (str[i] == '\\')
        {
            str = insert_string(str, strdup(""), i, i + 1);
        }
        else if (str[i] == '$')
        {
            j = i + 1;
            while (ft_isalpha(str[j]) || ft_isdigit(str[j]) || str[j] == '_' || str[j] == '?')
                j++;
            ptr = ft_strndup(str + i + 1, j - (i + 1));
			if ((elem = dlist_chr(envlist, ptr)) && elem->data->value && elem->data->value[0])
			{
            	str = insert_string(str, str_clean_whitespaces(elem->data->value), i, j);
				i += ft_strlen(elem->data->value) - 1;
			}
			else
			{
            	str = insert_string(str, strdup(""), i, j);
				i--;
			}
		}
		i++;
    }
	return (str);
}

int		token_push_back(t_tok **begin, t_tok *token)
{
	t_tok *tmp;

	if (!begin || !token)
		return (0);
	if (!(*begin))
	{
		*begin = token;
		return (1);
	}
	tmp = (*begin);
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->next = token;
	token->prev = tmp;
	return (1);
}

t_tok	*get_env_var_tok(const char *value)
{
	t_tok	*tok_var;
	t_tok	*token;
	char	*str;
	int		i;
	int		j;

	tok_var = NULL;
	if (!value)
	{
		tok_var = create_tok(CHR_ST, NULL);
		return (tok_var);
	}
	if (!*value)
	{
		tok_var = create_tok(CHR_ST, ft_strdup(""));
		return (tok_var);
	}
	i = 0;
	while (value[i])
	{
		if (ft_isspace(value[i]))
		{
			token = create_tok(CHR_SP, NULL);
			if (!token_push_back(&tok_var, token))
				return (NULL);
			while (ft_isspace(value[i]))
				i++;
		}
		else
		{
			j = i;
			while (value[j] && !ft_isspace(value[j]))
				j++;
			str = ft_strndup(value + i, j - i);
			if (!str)
				return (NULL);
			token = create_tok(CHR_ST, str);
			if (!token_push_back(&tok_var, token))
				return (NULL);
			i = j;
		}
	}
	return (tok_var);
}

t_tok	*get_word_tok(t_tok	*tok_lex, t_cmd	*cmd)
{
	char	*str;
	t_tok	*tok_word;
	t_tok	*token;
	t_dlist	*elem;
	char	*var_name;
	int		i;
	int		j;
	int		k;

	str = ft_strndup(tok_lex->value, ft_strlen((char*)tok_lex->value));
	if (!str)
		return (NULL);
	i = 0;
	k = 0;
	tok_word = NULL;
	while (str[i])
	{
		if (str[i] == '\\')
        {
            str = insert_string(str, strdup(""), i, i + 1);
			if (!str)
				return (NULL);
		}
		else if (str[i] == '$')
		{
			j = i + 1;
			while (ft_isalnum(str[j]) || str[j] == '_' || str[j] == '?')
				j++;
			if (j == i + 1)
			{
				i++;
				continue;
			}
			if (i - k > 0)
			{
				token = create_tok(CHR_ST, ft_strndup(str + k, i - k));
				if (!token)
					return (NULL);
				if (!token_push_back(&tok_word, token))
					return (NULL);
			}
			var_name = ft_strndup(str + i + 1, j - (i + 1));
			if (!var_name)
				return (NULL);
			elem = dlist_chr(cmd->envlist, var_name);
			if (!elem)
			{
				token = create_tok(CHR_ST, NULL);
				if (!token)
					return (NULL);
				if (!token_push_back(&tok_word, token))
					return (NULL);
			}
			else
			{
				token = get_env_var_tok((char*)elem->data->value);
				if (!token)
					return (NULL);
				if (!token_push_back(&tok_word, token))
					return (NULL);
			}
			i = j - 1;
			k = j;
		}
		i++;
	}
	if (i - k > 0)
	{
		token = create_tok(CHR_ST, ft_strndup(str + k, i - k));
		if (!token)
			return (NULL);
		if (!token_push_back(&tok_word, token))
			return (NULL);
	}
	free(str);
	return (tok_word);
}

t_tok	**get_tok_arg(t_tok **tok_lex, t_cmd *cmd)
{
	char 	*str;
	t_tok	*token;

	if (!tok_lex || !(*tok_lex))
		return (NULL);
	while ((*tok_lex)->type > CHR_ERROR && (*tok_lex)->type < CHR_PV && (*tok_lex)->type != CHR_PI)
		{
			if ((*tok_lex)->type == CHR_SP)
			{
				token = create_tok(CHR_SP, NULL);
				if (token)
				{
					if (!token_push_back(&cmd->tok_arg, token))
						return (NULL);
				}
				else
					return (NULL);
			}
			else if ((*tok_lex)->type == CHR_ST)
			{
				str = ft_strndup((*tok_lex)->value + 1, ft_strlen((*tok_lex)->value) - 2);
				if (str)
				{
					token = create_tok(CHR_ST, str);
					if (token)
						token_push_back(&cmd->tok_arg, token);
					else	
						return (NULL);
				}
				else
					return (NULL);
				
			}
			else if ((*tok_lex)->type == CHR_STR)
			{
				str = get_str(ft_strndup((*tok_lex)->value + 1, ft_strlen((*tok_lex)->value) - 2), cmd->envlist);
				if (str)
				{
					token = create_tok(CHR_ST, str);
					if (token)
						token_push_back(&cmd->tok_arg, token);
					else
						return (NULL);
				}
				else
					return (NULL);				
			}
			else if ((*tok_lex)->type == CHR_WORD)
			{
				token = get_word_tok(*tok_lex, cmd);
				if (token)
					token_push_back(&cmd->tok_arg, token);
				else
					return (NULL);
			}
			else
			{
				token = create_tok((*tok_lex)->type, ft_strdup((*tok_lex)->value));
				if (token)
					token_push_back(&cmd->tok_arg, token);
				else
					return (NULL);
				
			}			
			tok_lex++;
		}
	token_push_back(&cmd->tok_arg, create_tok(CHR_END, ft_strdup("newline")));
	return (tok_lex);
}

void	get_ac_av(t_tok *tok_lst,  t_cmd *cmd, int lvl)
{
	char	*s1 = NULL;
	char	*s2 = NULL;

	if (!tok_lst)
		return;
	if (tok_lst->type != CHR_END && tok_lst->type > CHR_ERROR &&  tok_lst->type < CHR_OP)
	{
		while (tok_lst->type == CHR_SP)
			tok_lst = tok_lst->next;
		while (tok_lst && tok_lst->type == CHR_ST)
		{
			if (tok_lst->value)
			{
				s2 = ft_strdup((char*)tok_lst->value);
				s1 = ft_strjoindoublefree(s1, s2);
			}
			tok_lst = tok_lst->next;
		}
		if (s1 && tok_lst->type != CHR_END && tok_lst->type > CHR_ERROR &&  tok_lst->type < CHR_OP)
			get_ac_av(tok_lst->next, cmd, lvl + 1);
		else if (s1)
			get_ac_av(tok_lst, cmd, lvl + 1);
		else
			get_ac_av(tok_lst, cmd, lvl);

	}
	else
	{
		cmd->av = (char**)malloc(sizeof(char*) * (lvl + 1));
		cmd->av[lvl] = NULL;
		cmd->ac = lvl;
		return;
	}
	if (s1)
		cmd->av[lvl] = s1;
	return;
}
