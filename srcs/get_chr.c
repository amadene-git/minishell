/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_chr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <admadene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 21:04:15 by admadene          #+#    #+#             */
/*   Updated: 2021/03/19 11:34:45 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	g_get_chr(char c)
{
	if (c == '!' || (c >= '#' && c <= '%') || (c >= '*' && c <= ':') ||\
	(c >= '?' && c <= '{') || (c >= '}' && c <= '~') || c == '=')
		return (CHR_WORD);
	if (c == '"')
		return (CHR_STR);
	if (c == '\'')
		return (CHR_ST);
	if (c == '\n' || c == '\v' || c == '\t' || c == '\r' || c == '\f' ||\
	c == ' ')
		return (CHR_SP);
	if (c == '&')
		return (CHR_OP);
	if (c == '|')
		return (CHR_PI);
	if (c == '>' || c == '<')
		return (CHR_RE);
	if (c == ';')
		return (CHR_PV);
	if (c == '\0')
		return (CHR_END);
	return (CHR_ERROR);
}
