/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_va_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 02:21:06 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/03 12:38:49 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_dprintf.h"

static	char	*format_width_str(t_flag flag, int len, char *str)
{
	char	*added;
	char	*pointerclean;

	added = ft_strnew(flag.width - len);
	ft_memset(added, flag.zeroenabled ? '0' : ' ', flag.width - len);
	pointerclean = str;
	if (flag.justify == 0)
		str = ft_strnjoin(added, str, len);
	else
		str = ft_strnjoin(str, added, flag.width - len);
	free(pointerclean);
	free(added);
	return (str);
}

static	char	*convert_to_char_str(wchar_t *str)
{
	char	*converted;
	char	*tmp;
	char	*pointerclean;
	int		i;

	if ((converted = ft_strnew(1)) == NULL)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if ((tmp = ft_strnew(1)) == NULL)
			return (NULL);
		tmp[0] = str[i];
		pointerclean = converted;
		converted = ft_strjoin(converted, tmp);
		free(tmp);
		free(pointerclean);
		i++;
	}
	return (converted);
}

static	char	*get_arg(va_list *ap, t_flag flag)
{
	char	*str;
	char	*arg;

	arg = va_arg(*ap, char *);
	if (arg == NULL)
		str = ft_strdup("(null)");
	else if (flag.modifier == 'l')
		str = convert_to_char_str((wchar_t *)arg);
	else
		str = ft_strdup(arg);
	return (str);
}

int				print_va_str(va_list *ap, t_flag flag, const int fd)
{
	int		len;
	char	*str;
	char	*pointerclean;

	str = get_arg(ap, flag);
	if (flag.precisionenabled && flag.precision >= 0)
	{
		pointerclean = str;
		str = ft_substr(str, 0, flag.precision);
		free(pointerclean);
	}
	if (str == NULL)
		return (-1);
	len = ft_strlen(str);
	if (flag.width < 0)
		flag.width = -flag.width;
	if (flag.widthenabled && flag.width - len > 0)
		str = format_width_str(flag, len, str);
	ft_putstr_fd(str, fd);
	free(str);
	return (flag.width - len > 0 ? flag.width : len);
}
