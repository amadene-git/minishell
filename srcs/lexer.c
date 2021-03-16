/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 14:43:12 by admadene          #+#    #+#             */
/*   Updated: 2021/03/16 14:46:07 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		lexer_one(char *str, int *i, int *j, int *type)
{
	if (g_get_chr[(int)str[*i]] == CHR_STR)
	{
		while (g_get_chr[(int)str[++(*j)]] != CHR_STR && str[(*j)])
			if (str[(*j)] == '\\' && str[(*j) + 1])
				(*j)++;
		if (str[(*j)])
			(*j)++;
		else
			*type = CHR_ERROR;
		return (1);
	}
	else if (g_get_chr[(int)str[*i]] == CHR_ST)
	{
		(*j) += 1;
		while (g_get_chr[(int)str[(*j)]] != CHR_ST && str[(*j)])
			(*j)++;
		if (str[(*j)])
			(*j)++;
		else
			*type = CHR_ERROR;
		return (1);
	}
	return (0);
}

int		lexer_two(char *str, int *i, int *j, int *type)
{
	if (g_get_chr[(int)str[*i]] > CHR_SP &&\
	g_get_chr[(int)str[*i]] < CHR_END)
	{
		(*j)++;
		if (g_get_chr[(int)str[*i]] == CHR_PI)
			*type = CHR_PI;
		else if (g_get_chr[(int)str[*i]] == CHR_OP)
			*type = CHR_ERROR;
		if (g_get_chr[(int)str[*i]] == CHR_RE)
		{
			*type = CHR_RE;
			if (!ft_strncmp(str + *i, ">>", 2) ||\
			!ft_strncmp(str + *i, "<<", 2))
				(*j)++;
			else if (!ft_strncmp(str + *i, "<>", 2) ||\
			!ft_strncmp(str + *i, "><", 2))
				*type = CHR_ERROR;
		}
		if (g_get_chr[(int)str[*i]] == CHR_PV)
			*type = CHR_PV;
		return (1);
	}
	return (0);
}

int		lexer_three(char *str, int *i, int *j, int *type)
{
	while (g_get_chr[(int)str[*i]] == g_get_chr[(int)str[(*j)]])
	{
		if (str[(*j)] == '\\' && str[(*j) + 1])
			(*j)++;
		if (str[(*j)])
			(*j)++;
		else
			*type = CHR_ERROR;
	}
	return (1);
}

t_tok	**lexer(char *str, int *i, int lvl)
{
	t_tok	**tab;
	char	*value;
	int		type;
	int		j;

	j = *i;
	type = CHR_END;
	if (!lexer_one(str, i, &j, &type) && !lexer_two(str, i, &j, &type))
		lexer_three(str, i, &j, &type);
	if (g_get_chr[(int)str[*i]] != CHR_SP)
		value = ft_strndup(str + *i, j - *i);
	else
		value = NULL;
	if (type != CHR_ERROR)
		type = g_get_chr[(int)str[*i]];
	*i = j;
	if (g_get_chr[(int)str[j]] != CHR_END)
		tab = lexer(str, i, lvl + 1);
	else
	{
		tab = (t_tok**)malloc(sizeof(t_tok*) * (lvl + 2));
		tab[lvl + 1] = create_tok(CHR_END, ft_strdup("newline"));
	}
	tab[lvl] = create_tok(type, value);
	return (tab);
}
