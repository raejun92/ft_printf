#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define conversions "cspdiuxX%"

static int g_rst;

typedef struct	s_flags
{
	int	minus;
	int dot;
	int zero;
	int width;
}				t_flags;

size_t	ft_strlen(const char *s)
{
	size_t i;
	i = 0;
	while (s[i])
		i++;
	return (i);
}

static void		ft_putchar(char c)
{
	write(1, &c, 1);
	g_rst += 1;
}

static void		ft_putstr(char *s)
{
	int len;

	if (!s)
		return ;
	len = ft_strlen(s);
	write(1, s, len);
	g_rst += len;
}

int		ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static int		ft_strchr(const char *s, int c) // 혹시 주소값으로 반환하면 if문에서 걸릴까봐 int형으로 바꿔줌
{
	char	*str;

	str = (char *)s;
	while (*str)
	{
		if (*str == (char)c)
			return (1);
		str++;
	}
	if ((char)c == '\0')
		return (1);
	return (0);
}

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
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
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
	flags->dot = -1; // 디폴트 -1, 0일 때 .만 .뒤에 숫자는 그대로 숫자 입력
	flags->zero = 0;
	flags->width = 0;
}

static void		is_flag(char fmt, t_flags *flags)
{
	if (fmt == '0' && flags->dot == -1 && flags->width == 0)
		flags->zero = 1;
	else if (fmt == '-' && flags->dot == -1 && flags->width == 0)
		flags->minus = 1;
}

static void		is_width(char fmt, t_flags *flags, va_list ap)
{
	if (flags->dot == -1 && (ft_isdigit(fmt) || fmt == '*'))
	{
		if (fmt == '*') // *로 width값이 들어올 때
		{
			flags->width = va_arg(ap, int);
			if (flags->width < 0) // *가 -값이면 -는 -플래그가 되고 숫자만 width가 됨
			{
				flags->minus = 1;
				flags->width *= -1;
			}
		}
		else
			flags->width = (flags->width * 10) + (fmt - '0');
	}
}

static void		is_precision(char fmt, t_flags *flags, va_list ap)
{
	if (fmt == '.' || (flags->dot >= 0 && (ft_isdigit(fmt) || fmt == '*')))
	{
		flags->zero = 0; // precision이 켜지면 0플래그는 꺼짐
		if (fmt == '.')
			flags->dot = 0;
		else if (fmt == '*')
			flags->dot = va_arg(ap, int);
		else
			flags->dot = (flags->dot * 10) + (fmt - '0');
	}
}

static void		flags_check(va_list ap, const char *fmt, t_flags *flags, int *i) // [flag][width][precision]각각 함수로 나눠야 함
{
	while (fmt[*i] && !(ft_strchr(conversions, fmt[*i]))) // cspdiuxX%이 안나오면 갇힐 수 있나?
	{
		is_flag(fmt[*i], flags);
		is_width(fmt[*i], flags, ap);
		is_precision(fmt[*i], flags, ap);
		(*i)++;
	}
	(*i)++;
}

static void		print_d(va_list ap, t_flags *flags) // precision, flags, width 처리 분리
{
	char *str;
	char *buf;
	char *remove_buf;
	int len;
	int j;

	j = 0;
	buf = ft_itoa(va_arg(ap, int));
	len = ft_strlen(buf);
	str = 0;
	// precision 처리
	if (flags->dot > len - 1 && buf[0] == '-') // 값이 음수
	{
		flags->dot = flags->dot - (len - 1); // -부호 뺀 길이
		str = (char *)malloc(sizeof(char) * (flags->dot + len + 1));
		str[flags->dot + len] = '\0';
		str[j++] = '-';
		while (flags->dot--)
			str[j++] = '0';
		remove_buf = buf;
		while (*(buf++) != '\0')
			str[j++] = *buf;
		free(remove_buf);
		remove_buf = 0;
	}
	else if (flags->dot > len && buf[0] != '-') // 값이 양수
	{
		flags->dot = flags->dot - len;
		str = (char *)malloc(sizeof(char) * (flags->dot + len + 1));
		while (flags->dot--)
			str[j++] = '0';
		remove_buf = buf;
		while (*(buf) != '\0')
			str[j++] = *buf++;
		free(remove_buf);
		remove_buf = 0;
	}
	else // precision 없음
		str = buf;
	// flags & width 처리
	len = ft_strlen(str); // precision이 포함된 값의 길이
	if (flags->minus == 1) // -플래그
	{
		ft_putstr(str);
		while (flags->width-- > len)
			ft_putchar(' ');
	}
	else
	{
		while (flags->width-- > len)
		{
			if (flags->zero == 0)
				ft_putchar(' ');
			else
				ft_putchar('0');
		}
		ft_putstr(str);
	}
	free(str);
}

static int		format_specifier(va_list ap, char c, t_flags *flags) //cspdiuxX%
{
	if (c == 'd')
		print_d(ap, flags);
	else
		return (0);
	return (0);
}

static void		ft_vsprintf(va_list ap, const char *fmt)
{
	int			i;
	t_flags		*flags;
	
	flags = (t_flags *)malloc(sizeof(t_flags) * 1);
	if (!flags)
	{
		g_rst = -1;
		return ;
	}
	i = 0;
	while (fmt[i])
	{
		if (fmt[i] == '%')
		{
			init_flags(flags);
			flags_check(ap, &fmt[i], flags, &i);
			i += format_specifier(ap, fmt[i], flags);
		}
		else
			ft_putchar(fmt[i]);
		i++;
	}
	free(flags);
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
	int a;

 	// printf("|%08.6d|\n", -12345);
	a = printf("|%.6d|\n", -12345);
	printf("a = %d\n", a);
	// printf("|%06d|\n", -12345);
	// printf("|%-7.9d|\n", -12345);
	// printf("|%09d|\n", -12345);
	// printf("|%0*d|\n", -9, -12345);
	
	printf("\n");

	 
	// ft_printf("|%08.6d|\n", -12345);
	a = ft_printf("|%.6d|\n", -12345);
	printf("a = %d\n", a);
	// ft_printf("|%06d|\n", -12345);
	// ft_printf("|%-7.9d|\n", -12345);
	// ft_printf("|%09d|\n", -12345);
	// printf("|%0*d|\n", -9, -12345);

	//system("leaks a.out > leaks_result_temp; cat leaks_result_temp | grep leaked && rm -rf leaks_result_temp");
}