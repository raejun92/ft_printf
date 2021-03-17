#ifndef FT_PRINTF_H
# define FT_PRINTF_H

#include <stdarg.h>
#include <stdio.h>   // 제출 전 수정 필요.
#include <unistd.h>
#include <stdlib.h>

#define conversions "cspdiuxX%"

int g_rst;

typedef struct	s_flags
{
	int	minus;
	int dot;
	int zero;
	int width;
}				t_flags;

size_t	ft_strlen(const char *s);
void		ft_putchar(char c);
void		ft_putchar_base(char c, int len);
void		ft_putstr(char *s);
void		ft_putstr_base(char *s, int len);
int		ft_isdigit(int c);
int		ft_strchr(const char *s, int c);
int		ft_intlen(long long n, int base);
char			*ft_itoa(int n);
char		*ft_ltoa(long long n);
void		init_flags(t_flags *flags);
void		is_flag(char fmt, t_flags *flags);
void		is_width(char fmt, t_flags *flags, va_list ap);
void		is_precision(char fmt, t_flags *flags, va_list ap);
void		flags_check(va_list ap, const char *fmt, t_flags *flags, int *i);
char		*proc_precision(char *str, t_flags *flags, int value);
void		print_c(char c, t_flags *flags);
void		print_s(char *s, t_flags *flags);
int			zero_number(int num, t_flags *flags) ;
int			blank_number(int num, t_flags *flags, int zero_num);
void		blank_output(int width_num);
void		number_output(int num, int zero_num, t_flags *flags);
void		print_d(int num, t_flags *flags);
void		print_p(long long pointer, t_flags *flags);
void		format_specifier(va_list ap, char c, t_flags *flags);
void		ft_vsprintf(va_list ap, const char *fmt);
int				ft_printf(const char *fmt, ...);

#endif
