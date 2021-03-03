/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_width_dec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 17:37:37 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/03 12:26:07 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

int		format_width_dec(t_flag flag, char **str, int len)
{
	char	*added;
	char	*pointerclean;

	if (flag.zeroenabled && flag.justify == 0 && (flag.precision < 0 \
		|| !flag.precisionenabled))
	{
		flag.precisionenabled = 1;
		flag.precision = flag.width;
		if (*str[0] == '-' || *str[0] == '+' || *str[0] == ' ')
			flag.precision--;
		format_precision(str, flag);
	}
	else
	{
		added = ft_strnew(flag.width - len);
		ft_memset(added, ' ', flag.width - len);
		pointerclean = *str;
		if (flag.justify == 0)
			*str = ft_strnjoin(added, *str, len);
		else
			*str = ft_strnjoin(*str, added, flag.width - len);
		free(pointerclean);
		free(added);
	}
	return (flag.width);
}
