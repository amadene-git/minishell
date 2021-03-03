/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_va_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 02:23:18 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/03 12:44:17 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

int		print_va_char(va_list *ap, t_flag flag, const int fd)
{
	int		len;
	char	c;

	if (flag.modifier == 'l')
		c = (wchar_t)va_arg(*ap, int);
	else
		c = va_arg(*ap, int);
	if (!c)
		c = 0;
	if (flag.width < 0)
		flag.width = -flag.width;
	len = 1;
	if (flag.widthenabled && flag.width - len > 0)
		len += format_width_char(flag, c, fd);
	else
		ft_putchar_fd(c, fd);
	return (len);
}
