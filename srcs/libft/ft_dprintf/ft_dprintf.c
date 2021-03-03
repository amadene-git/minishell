/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/20 15:43:33 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/03 12:29:58 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

int		ft_dprintf(const int fd, const char *format, ...)
{
	int		pos;
	int		flagssize;
	int		handlersize;
	int		lastpos;
	va_list	ap;

	va_start(ap, format);
	pos = 0;
	flagssize = 0;
	handlersize = 0;
	while (format[pos])
	{
		if (format[pos] == '%')
		{
			lastpos = pos;
			if ((handlersize = handle_flag(format, &pos, &ap, fd)) == -1)
				return (-1);
			else
				flagssize += handlersize - (pos - lastpos) - 1;
		}
		else if (format[pos] != '%')
			ft_putchar_fd(format[pos], fd);
		pos++;
	}
	return (pos + flagssize);
}
