#include "ft_printf.h"

// 기능 : 플래그초기화, 리턴 : void
static void	init_flags(t_flags *flags) 
{
	flags->minus = 0;
	flags->zero = 0;
	flags->width = 0;
	flags->dot = -1;
}

// 기능 : flags, width, precision이 존재하는지 확인, 리턴 : void
static void	flags_check(va_list ap, const char *fmt, t_flags *flags, int *i) 
{
	while (fmt[++(*i)] && !(ft_strchr(conversions, fmt[*i])))
	{
		is_flag(fmt[*i], flags);
		is_width(fmt[*i], flags, ap);
		is_precision(fmt[*i], flags, ap);
	}
	if (flags->minus == 1 && flags->zero == 1) // (-)플래그와 (0)플래그가 켜져 있으면 (0)플래그 끔
		flags->zero = 0;
}

// 기능 : 서식지정자에 따라 출력 함수 처리가 다름, 리턴 : void
static void	format_specifier(va_list ap, char c, t_flags *flags)
{
	char	*base;

	base = 0;
	if (c == 'c')
		print_c(va_arg(ap, int), flags);
	else if (c == 's')
		print_s(va_arg(ap, char *), flags);
	else if (c == '%')
		print_percent(flags);
	else if (c == 'd' || c == 'i')
		print_d(va_arg(ap, int), flags);
	else if (c == 'u')
		print_u(va_arg(ap, unsigned int), flags);
	else if (c == 'p')
		print_p(va_arg(ap, long long), flags);
	else if (c == 'x' || c == 'X')
	{
		if (c == 'x')
			base = "0123456789abcdef";
		else
			base = "0123456789ABCDEF";
		print_x(va_arg(ap, unsigned int), flags, base);
	}
	else
		return ;
}

// 기능 : 일반 문자와 %서식지정자를 나누어 출력, 리턴 : void
static void	ft_vsprintf(va_list ap, const char *fmt)
{
	int		i;
	t_flags	*flags;

	flags = (t_flags *)malloc(sizeof(t_flags) * 1);
	if (!flags)
	{
		g_rst = -1;
		return ;
	}
	i = -1;
	while (fmt[++i])
	{
		if (fmt[i] == '%')
		{
			init_flags(flags);
			flags_check(ap, fmt, flags, &i);
			format_specifier(ap, fmt[i], flags);
		}
		else
			ft_putchar(fmt[i]);
	}
	free(flags);
}

int			ft_printf(const char *fmt, ...)
{
	va_list ap; // 가변인자를 다 담음
	
	g_rst = 0;
	va_start(ap, fmt); // fmt는 마지막 고정인수 
	ft_vsprintf(ap, fmt);
	va_end(ap);
	return (g_rst);
}