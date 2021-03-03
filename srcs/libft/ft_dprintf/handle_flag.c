/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_flag.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/26 15:48:54 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/03 12:34:41 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

static	int		findindex(char *tab, char element)
{
	int i;

	i = 0;
	while (tab[i])
	{
		if (tab[i] == element)
			return (i);
		i++;
	}
	return (-1);
}

static	void	fill_va_fncts(int (*print_va_fncts[9])(va_list *, t_flag flag,\
									const int fd))
{
	print_va_fncts[0] = print_va_str;
	print_va_fncts[1] = print_va_char;
	print_va_fncts[2] = print_va_dec;
	print_va_fncts[3] = print_va_hex;
	print_va_fncts[4] = print_va_hex;
	print_va_fncts[5] = print_va_hex;
	print_va_fncts[6] = print_va_udec;
	print_va_fncts[7] = print_va_dec;
	print_va_fncts[8] = print_va_percent;
}

static	void	init_t_flag(t_flag *flag)
{
	flag->justify = 0;
	flag->width = 0;
	flag->precision = 0;
	flag->modifier = 0;
	flag->type = 0;
	flag->widthenabled = 0;
	flag->precisionenabled = 0;
	flag->zeroenabled = 0;
	flag->signenabled = 0;
	flag->spaceenabled = 0;
}

static	t_flag	fill_flag_data(const char *str, int *pos, va_list *ap)
{
	t_flag		new;

	init_t_flag(&new);
	handle_params(&new, str, pos);
	if (str[*pos] == '*')
	{
		new.width = fill_int_star(&(new.widthenabled), ap, pos);
		if (new.width < 0 && new.justify == 0)
			new.justify = 1;
	}
	else
		new.width = capture_int_str(str, pos, &(new.widthenabled));
	if (str[*pos] == '.')
		handle_precision(str, pos, ap, &new);
	handle_modifiers(&new, str, pos);
	if (ft_isalpha(str[*pos]) || str[*pos] == '%')
		new.type = str[*pos];
	else
		new.type = 0;
	return (new);
}

int				handle_flag(const char *format, int *pos, va_list *ap,\
							const int fd)
{
	int		tmp;
	int		(*print_va_fncts[9])(va_list *, t_flag flag, const int fd);
	char	*tab;
	t_flag	flag;

	fill_va_fncts(print_va_fncts);
	tab = (char[10]) {'s', 'c', 'd', 'p', 'x', 'X', 'u', 'i', '%', 0};
	flag = fill_flag_data(format, pos, ap);
	tmp = findindex(tab, format[*pos]);
	if (tmp != -1)
		return ((*print_va_fncts[tmp])(ap, flag, fd));
	else
		return (-1);
}
