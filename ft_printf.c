#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

static unsigned int g_rst;

typedef struct	s_flags
{
	int	minus;
	int	star;
	int dot;
	int zero;
	int sharp;
	int plus;
	int blank;
}				t_flags;

static void		init_flags(t_flags *flags)
{
	flags->minus = 0;
	flags->star = 0;
	flags->dot = 0;
	flags->zero = 0;
	flags->sharp = 0;
	flags->plus = 0;
	flags->blank = 0;
}

static void		ft_putchar_len(char c, int len)
{
	write(1, &c, len);
	g_rst += len;
}

static int		format_specifier(va_list ap, char c)
{
	if (c == 'c')
		print_c(ap);
	else if (c == 's')
		print_s(ap);
	else if (c == 'd')
		print_d(ap);
	else
		return (0);
	return (1);
}

static void		ft_vsprintf(va_list ap, const char *fmt)
{
	unsigned int i;
	t_flags *flags;
	
	i = 0;
	while (fmt[i])
	{
		if (fmt[i] =='%')
		{
			init_flags(flags);
			i += fotmat_specifier(ap, fmt[i]);
		}
		else
			ft_putchar_len(fmt[i], 1);
		i++;
	}
}

int		ft_printf(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	ft_vsprintf(ap, fmt);
	va_end(ap);
	return (g_rst);
}

int main()
{
		
}