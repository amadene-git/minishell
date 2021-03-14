/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/20 16:30:39 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/14 16:49:07 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DPRINTF_H
# define FT_DPRINTF_H

# include <stdarg.h>
# include "libft.h"

typedef	struct		s_flag
{
	int				justify;
	char			type;
	int				width;
	int				precision;
	int				modifier;
	size_t			zeroenabled;
	size_t			widthenabled;
	size_t			precisionenabled;
	size_t			signenabled;
	size_t			spaceenabled;
}					t_flag;

int					ft_dprintf(const int fd, const char *format, ...);
int					print_va_char(va_list *ap, t_flag flag, const int fd);
int					print_va_dec(va_list *ap, t_flag flag, const int fd);
int					print_va_str(va_list *ap, t_flag flag, const int fd);
int					print_va_hex(va_list *ap, t_flag flag, const int fd);
int					print_va_udec(va_list *ap, t_flag flag, const int fd);
int					print_va_percent(va_list *ap, t_flag flag, const int fd);
int					handle_flag(const char *format, int *pos, va_list *ap,\
								const int fd);
void				handle_params(t_flag *flag, const char *str, int *pos);
void				handle_precision(const char *str, int *pos, va_list *ap, \
								t_flag *flag);
void				handle_modifiers(t_flag *flag, const char *str, int *pos);
int					fill_int_star(size_t *enablepart, va_list *ap, int *pos);
int					capture_int_str(const char *str, int *pos, size_t *enable);
int					format_width_dec(t_flag flag, char **str, int len);
int					format_width_char(t_flag flag, const char c, const int fd);
void				format_precision(char **str, t_flag flag);

#endif
