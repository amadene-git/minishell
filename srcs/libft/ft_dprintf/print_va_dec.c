/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_va_dec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 02:24:09 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/03 12:38:10 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

static	intmax_t	get_arg(va_list *ap, t_flag flag)
{
	if (flag.modifier == 0)
		return (va_arg(*ap, int));
	else if (flag.modifier == 'h')
		return ((short)va_arg(*ap, int));
	else if (flag.modifier == 'h' + 'h')
		return ((char)va_arg(*ap, int));
	else if (flag.modifier == 'l')
		return (va_arg(*ap, long));
	else if (flag.modifier == 'l' + 'l')
		return (va_arg(*ap, long long));
	else
		return (0);
}

int					print_va_dec(va_list *ap, t_flag flag, const int fd)
{
	int			len;
	char		*str;
	char		*clearpointer;
	intmax_t	arg;

	arg = get_arg(ap, flag);
	str = ft_itoa(arg);
	if ((flag.signenabled || flag.spaceenabled) && arg >= 0)
	{
		clearpointer = str;
		if (flag.signenabled && arg >= 0)
			str = ft_strjoin("+", str);
		if (flag.spaceenabled && arg >= 0)
			str = ft_strjoin(" ", str);
		free(clearpointer);
	}
	format_precision(&str, flag);
	if (flag.width < 0)
		flag.width = -flag.width;
	len = ft_strlen(str);
	if (flag.widthenabled && flag.width - len > 0)
		len = format_width_dec(flag, &str, len);
	ft_putstr_fd(str, fd);
	free(str);
	return (len);
}
