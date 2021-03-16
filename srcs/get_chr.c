/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_chr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <admadene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 21:04:15 by admadene          #+#    #+#             */
/*   Updated: 2021/03/16 21:05:11 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	g_get_chr(char c)
{
	char get_chr[255] = {
		['!'] = CHR_WORD,
		['#'...'%'] = CHR_WORD,
		['*'...':'] = CHR_WORD,
		['?'...'{'] = CHR_WORD,
		['}'...'~']	= CHR_WORD,
		['='] = CHR_WORD,
		['"'] = CHR_STR,
		['\''] = CHR_ST,
		['\n'] = CHR_SP,
		['\t'] = CHR_SP,
		['\v'] = CHR_SP,
		['\r'] = CHR_SP,
		['\f'] = CHR_SP,
		[' '] = CHR_SP,
		['&'] = CHR_OP,
		['|'] = CHR_PI,
		['>'] = CHR_RE,
		['<'] = CHR_RE,
		[';'] = CHR_PV,
		['\0'] = CHR_END
	};
	return (get_chr[(int)c]);
}
