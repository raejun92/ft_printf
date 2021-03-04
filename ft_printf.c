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

static int		ft_intlen(int n)
{
	int i;

	i = 0;
	if (n <= 0)
		i = i + 1;
	while (n != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char			*ft_itoa(int n)
{
	char		*str;
	int			len;
	int			i;
	long long	ln;

	ln = (long long)n;
	len = ft_intlen(n);
	if (!(str = (char *)malloc(sizeof(char) * (len + 1))))
		return (0);
	str[len--] = '\0';
	i = 0;
	if (ln < 0)
	{
		ln = ln * -1;
		str[i++] = '-';
	}
	while (i <= len)
	{
		str[len] = (ln % 10) + '0';
		ln = ln / 10;
		len--;
	}
	return (str);
}

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

static int		only_flags(char c, t_flags *flags)
{
	if (c == '0')
		flags->zero = 1;
	else if (c == '-')
		flags->minus = 1;
	else if (c == '.')
		flags->dot = 1;
	else if (c == '*')
		flags->star = 1;
	else if (c == '+')
		flags->plus = 1;
	else if (c == ' ')
		flags->blank = 1;
	else if (c == '#')
		flags->sharp = 1;
	else
		return (0);
	return (1);
}

static void		flags_check(char *fmt, t_flags *flags, int *i)
{
	while (fmt[(*i)])
	{

	}
}

void			ft_putchar(char c)
{
	write(1, &c, 1);
	g_rst += 1;
}

void			ft_putstr_fd(char *s, int fd)
{
	int len;

	if (!s)
		return ;
	len = ft_strlen(s);
	write(fd, s, len);
	g_rst += len;
}

static void		print_d(va_list ap, t_flags *flags)
{
	char *str;
	
	str = ft_itoa(va_arg(ap, int));
	while (*str)
		ft_putchar(*str++);
	free(str);
	str = 0;
}

static int		format_specifier(va_list ap, char c, t_flags *flags)
{
	if (c == 'c')
		print_c(ap, flags);
	else if (c == 's')
		print_s(ap, flags);
	else if (c == 'd')
		print_d(ap, flags);
	else
		return (0);
	return (1);
}

static void		ft_vsprintf(va_list ap, const char *fmt)
{
	unsigned int i;
	unsigned int j;
	t_flags *flags;
	
	i = 0;
	while (fmt[i])
	{
		if (fmt[i++] =='%')
		{
			init_flags(flags);
			flags_check(&fmt[i], flags, &i);
			i += format_specifier(ap, fmt[i], flags);
		}
		else
			ft_putchar(fmt[i]);
		i++;
	}
}

int				ft_printf(const char *fmt, ...)
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