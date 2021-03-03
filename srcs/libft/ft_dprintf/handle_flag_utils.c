/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_flag_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/26 04:23:55 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/03 12:26:17 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

int		capture_int_str(const char *str, int *pos, size_t *enable)
{
	int		i;
	int		num;
	char	*numinstr;

	i = 0;
	num = 0;
	if (str[*pos] == '-')
		i++;
	if (ft_isdigit(str[*pos + i]))
	{
		*enable = 1;
		while (ft_isdigit(str[*pos + i]))
			i++;
		numinstr = ft_substr(str, *pos, i);
		num = atoi(numinstr);
		free(numinstr);
		*pos += i;
	}
	return (num);
}

int		fill_int_star(size_t *enablepart, va_list *ap, int *pos)
{
	int		n;

	n = va_arg(*ap, int);
	*enablepart = 1;
	(*pos)++;
	return (n);
}

void	handle_precision(const char *str, int *pos, va_list *ap, \
				t_flag *flag)
{
	(*pos)++;
	if (str[*pos] == '*')
		flag->precision = fill_int_star(&(flag->precisionenabled), ap, pos);
	else
	{
		flag->precision = capture_int_str(str, pos, &(flag->precisionenabled));
		if (flag->precisionenabled == 0)
			flag->precisionenabled = 1;
	}
}

void	handle_params(t_flag *flag, const char *str, int *pos)
{
	const char	*params = "-0 +";

	while (str[(*pos)++] && ft_strchr(params, str[*pos]))
	{
		if (str[*pos] == '-')
			flag->justify = 1;
		else if (str[*pos] == '0' && flag->justify != 1)
			flag->zeroenabled = 1;
		else if (str[*pos] == ' ')
			flag->spaceenabled = 1;
		else if (str[*pos] == '+')
			flag->signenabled = 1;
		else
		{
			(*pos)++;
			break ;
		}
	}
}

void	handle_modifiers(t_flag *flag, const char *str, int *pos)
{
	const char	*modifiers = "hl";

	while (str[*pos] && ft_strchr(modifiers, str[*pos]))
	{
		flag->modifier += str[*pos];
		(*pos)++;
	}
}
