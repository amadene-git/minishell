/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_width_char.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/16 19:51:47 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/03 12:44:05 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

int		format_width_char(t_flag flag, const char c, const int fd)
{
	char	*added;

	added = ft_strnew(flag.width - 1);
	ft_memset(added, (flag.zeroenabled && !flag.justify) ? '0' : ' ', \
			flag.width - 1);
	if (flag.justify == 0)
	{
		ft_putstr_fd(added, fd);
		ft_putchar_fd(c, fd);
	}
	else
	{
		ft_putchar_fd(c, fd);
		ft_putstr_fd(added, fd);
	}
	free(added);
	return (flag.width - 1);
}
