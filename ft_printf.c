#include "ft_printf.h"

size_t	ft_strlen(const char *s)
{
	size_t i;
	
	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

void		ft_putchar(char c)
{
	write(1, &c, 1);
	g_rst += 1;
}

void		ft_putchar_base(char c, int len)
{
	g_rst += len;
	while (len--)
		write(1, &c, 1);
}

void		ft_putstr(char *s)
{
	int len;

	if (!s)
		return ;
	len = ft_strlen(s);
	write(1, s, len);
	g_rst += len;
}

void		ft_putstr_base(char *s, int len)
{
	if (!s)
		return ;
	write(1, s, len);
	g_rst += len;
}

int		ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int		ft_strchr(const char *s, int c) // 혹시 주소값으로 반환하면 if문에서 걸릴까봐 int형으로 바꿔줌
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

int		ft_intlen(int n)
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

void		init_flags(t_flags *flags)
{
	flags->minus = 0;
	flags->dot = -1; // 디폴트 -1, 0일 때 .만 .뒤에 숫자는 그대로 숫자 입력
	flags->zero = 0;
	flags->width = 0;
}

void		is_flag(char fmt, t_flags *flags)
{
	if (fmt == '0' && flags->dot == -1 && flags->width == 0)
		flags->zero = 1;
	else if (fmt == '-' && flags->dot == -1 && flags->width == 0)
		flags->minus = 1;
}

void		is_width(char fmt, t_flags *flags, va_list ap)
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

void		is_precision(char fmt, t_flags *flags, va_list ap)
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

void		flags_check(va_list ap, const char *fmt, t_flags *flags, int *i) // [flag][width][precision]각각 함수로 나눠야 함
{
	while (fmt[++(*i)] && !(ft_strchr(conversions, fmt[*i]))) // cspdiuxX%이 안나오면 갇힐 수 있나?
	{
		is_flag(fmt[*i], flags);
		is_width(fmt[*i], flags, ap);
		is_precision(fmt[*i], flags, ap);
	}
}

char		*proc_precision(char *str, t_flags *flags, int value)
{
	char *buf;
	char *buf_del;
	int	len;
	int j;

	buf = ft_itoa(value);
	len = ft_strlen(buf);
	j = 0;
	if (flags->dot == 0 && value == 0)
		str = 0;
	else if (flags->dot >= len)
	{
		if (buf[0] == '-')
		{
			flags->dot = flags->dot - (len - 1); // -부호 뺀 길이
			str = (char *)malloc(sizeof(char) * (flags->dot + len + 1));
			str[flags->dot + len] = '\0';
			str[j++] = '-';
			while (flags->dot--)
				str[j++] = '0';
			buf_del = buf;
			while (*(buf++) != '\0')
				str[j++] = *buf;
			free(buf_del);
			buf_del = 0;
		}
		else
		{
			flags->dot = flags->dot - len;
			str = (char *)malloc(sizeof(char) * (flags->dot + len + 1));
			str[flags->dot + len] = '\0';
			while (flags->dot--)
				str[j++] = '0';
			buf_del = buf;
			while (*(buf) != '\0')
				str[j++] = *buf++;	
			free(buf_del);
			buf_del = 0;
		}
	}
	else
		str = buf;
	return (str);
}

void		print_c(char c, t_flags *flags)
{
	if (flags->minus == 1)
	{
		ft_putchar(c);
		while (--(flags->width) > 0)
			ft_putchar(' ');
	}
	else
	{
		while (--(flags->width) > 0)
			ft_putchar(' ');
		ft_putchar(c);
	}
}

void		print_s(char *s, t_flags *flags)
{
	int s_len;

	if (!s) // s에 NULL 또는 0이 들어왔을 때
		s = "(null)";
	s_len = ft_strlen(s);
	if (s_len > flags->dot && flags->dot >= 0) // precision이 0이상이고 값의 길이보다 작을 때
		s_len = flags->dot; // precision이 0일 때 아무것도 출력 안함
	if (flags->minus == 1)
	{
		ft_putstr_base(s, s_len);
		if (flags->width > s_len)
			ft_putchar_base(' ', (flags->width - s_len));
	}
	else
	{
		if (flags->width > s_len)
			ft_putchar_base(' ', (flags->width - s_len));
		ft_putstr_base(s, s_len);
	}
}

void		print_d(va_list ap, t_flags *flags) // precision, flags & width 순으로 처리
{
	char *str;
	int len;

	str = 0;
	// precision 처리
	str = proc_precision(str, flags, va_arg(ap, int)); 
	// flags & width 처리
	len = ft_strlen(str); // precision이 포함된 값의 길이
	if (flags->minus == 1) // -플래그이면 0플래그가 켜져있든 말든 상관 없이 들어옴
	{
		ft_putstr(str);
		while (flags->width-- > len)
			ft_putchar(' ');
	}
	else if (flags->zero == 1 && flags->minus == 0) // 0플래그일 때
	{
		if (str[0] == '-') // 값이 음수일 때 
		{
		
			ft_putchar(str[0]);
			while (flags->width-- > len)
				ft_putchar('0');
			ft_putstr((str + 1));
		}
		else // 값이 양수일 때
		{
			while (flags->width-- > len)
				ft_putchar('0');
			ft_putstr((str));
		}
	}
	else // 플래그가 없을 때
	{
		while (flags->width-- > len)
			ft_putchar(' ');
		ft_putstr(str);
	}
	free(str);
}

void		format_specifier(va_list ap, char c, t_flags *flags) //cspdiuxX%
{
	if (c == 'd')
		print_d(ap, flags);
	else if (c == 'c')
		print_c(va_arg(ap, int), flags);
	else if (c == 's')
		print_s(va_arg(ap, char *), flags);
	else
		return ;
}

void		ft_vsprintf(va_list ap, const char *fmt)
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
			flags_check(ap, fmt, flags, &i);
			format_specifier(ap, fmt[i], flags);
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
	g_rst = 0;
	va_start(ap, fmt);
	ft_vsprintf(ap, fmt);
	va_end(ap);
	return (g_rst);
}

// int main()
// {
// 	char *s = 0;
// 	int a;
// 	a = printf("-->|%-16.s|<--\n", s);
// 	printf("%d\n", a);
// 	printf("\n");
// 	ft_printf("-->|%-16.s|<--\n", s);
// 	printf("%d\n", a);
// 	// system("leaks a.out > leaks_result_temp; cat leaks_result_temp | grep leaked && rm -rf leaks_result_temp");
// }