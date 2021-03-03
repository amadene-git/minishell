/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_va_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 02:26:44 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/03 12:38:32 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

static	intmax_t	get_arg(va_list *ap, t_flag flag)
{
	if (flag.type == 'p')
		return (va_arg(*ap, unsigned long long));
	else if (flag.modifier == 0)
		return (va_arg(*ap, unsigned int));
	else if (flag.modifier == 'h')
		return ((unsigned short)va_arg(*ap, int));
	else if (flag.modifier == 'h' + 'h')
		return ((unsigned char)va_arg(*ap, int));
	else if (flag.modifier == 'l')
		return (va_arg(*ap, unsigned long));
	else if (flag.modifier == 'l' + 'l')
		return (va_arg(*ap, unsigned long long));
	else
		return (0);
}

static char			*init_str_flag_p(char **str)
{
	char	*hex;
	char	*pointerclean;

	hex = ft_strdup("0x");
	pointerclean = hex;
	hex = ft_strnjoin(hex, *str, ft_strlen(*str));
	free(pointerclean);
	free(*str);
	return (hex);
}

int					print_va_hex(va_list *ap, t_flag flag, const int fd)
{
	int			len;
	char		*str;
	uintmax_t	arg;

	arg = get_arg(ap, flag);
	str = ft_uitoa(arg, 16);
	format_precision(&str, flag);
	if (flag.type == 'p')
		str = init_str_flag_p(&str);
	if (flag.type == 'X')
		str = ft_strupr(str);
	len = ft_strlen(str);
	if (flag.width < 0)
		flag.width = -flag.width;
	if (flag.widthenabled && flag.width - len > 0)
		len = format_width_dec(flag, &str, len);
	ft_putstr_fd(str, fd);
	free(str);
	return (len);
}
