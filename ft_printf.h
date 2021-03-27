#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>

# define conversions "cspdiuxX%"

int g_rst;

typedef struct	s_flags
{
	int minus;
	int dot;
	int zero;
	int width;
}				t_flags;
// ft_printf.c
int			ft_printf(const char *fmt, ...);

// flags_func.c
void		is_flag(char fmt, t_flags *flags);
void		is_width(char fmt, t_flags *flags, va_list ap);
void		is_precision(char fmt, t_flags *flags, va_list ap);

// utils_func.c
void		ft_free(char *s);
size_t		ft_strlen(const char *s);
int			ft_isdigit(int c);
int			ft_strchr(const char *s, int c);

// output_func.c
void		ft_putchar(char c);
void		ft_putchar_base(char c, int len);
void		ft_putstr(char *s);
void		ft_putstr_base(char *s, int len);

// toa_func.c
char		*ft_itoa(int n);
char		*ft_ltoa(long long n);
char		*ft_uitoa_base(unsigned int n, char *base, int base_num);

// character_func.c
void		print_c(char c, t_flags *flags);
void		print_s(char *s, t_flags *flags);
void		print_percent(t_flags *flags);

// decimal_func.c
void		print_d(int num, t_flags *flags);
void		print_u(unsigned int num, t_flags *flags);

// hexadecimal_func.c
void		print_p(long long p, t_flags *flags);
int			zero_number_ui(char *s_num, t_flags *flags);
int			blank_number_ui(unsigned int num, char *s_num, t_flags *flags, int zero_num);
void		print_x(unsigned int num, t_flags *flags, char *base);

#endif