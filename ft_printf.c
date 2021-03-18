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

int		ft_intlen(long long n, int base)
{
	int i;

	i = 0;
	if (n <= 0)
		i = i + 1;
	while (n != 0)
	{
		n = n / base;
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
	len = ft_intlen(ln, 10);
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
		str[len--] = (ln % 10) + '0';
		ln = ln / 10;
	}
	return (str);
}

char		*ft_ltoa(long long n)
{
	char	*str;
	char	*base;
	int		len;
	int		i;

	base = "0123456789abcdef";
	if (n < 0)
		return (0);
	len = ft_intlen(n, 16);
	str = (char *)malloc(sizeof(char) * len + 1);
	if (!str)
		return (0);
	str[len--] = '\0';
	i = 0;
	while (i <= len)
	{
		str[len--] = base[n % 16];
		n = n / 16;
	}
	return (str);
}

char		*ft_uitoa_base(unsigned int n, char *base, int base_num)
{
	unsigned int n_tmp;
	int len;
	char *str;

	n_tmp = n;
	len = 0;
	if (n_tmp == 0)
		len++;
	while (n_tmp != 0)
	{
		n_tmp = n_tmp / base_num;
		len++;
	}
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (0);
	str[len--] = '\0';
	while (len >= 0)
	{
		str[len--] = base[n % base_num];
		n = n / base_num;
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
	{
		if (flags->zero == 1)
			flags->zero = 0;
		flags->minus = 1;
	}
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
	if (flags->minus == 1 && flags->zero == 1) // (-)플래그와 (0)플래그가 켜져 있으면 (0)플래그 꺼줌
		flags->zero = 0;
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

int			zero_number(int num, t_flags *flags) // 0플래그 또는 precision일 때 0패딩 발생
{
	char *s_num;
	int zero_num;
	int num_len;

	zero_num = 0;
	s_num = ft_itoa(num);
	num_len = ft_strlen(s_num);
	if (flags->zero == 1 && flags->width > num_len) // 0플래그인 경우(-포함한 길이)
		zero_num = flags->width - num_len;
	if (num <= 0) // -부호뺌, 값이 0일 땐 길이 0
		num_len--;
	if (flags->dot > 0 && flags->dot > num_len) // precision인 경우(-뺀 길이)
		zero_num = flags->dot - num_len;
	free(s_num);
	s_num = 0;
	return (zero_num);
}

int			blank_number(int num, t_flags *flags, int zero_num)
{
	int width_num;
	char *s_num;
	int num_len;

	width_num = 0;
	s_num = ft_itoa(num);
	num_len = ft_strlen(s_num);
	if (num == 0 && flags->dot >= 0) // 값이 0일 때 precision이 존재하면 값인 0이 출력 되지 않고 공백을 출력함
		num_len--;
	if (flags->width > (num_len + zero_num))
		width_num = flags->width - (num_len + zero_num);
	free(s_num);
	s_num = 0;
	return (width_num);
}

void		blank_output(int width_num)
{
	ft_putchar_base(' ', width_num);
}

void		number_output(int num, int zero_num, t_flags *flags)
{
	char *s_num;

	if (flags->dot == 0 && num == 0) // 값이 0이고 precision이 0이면 아무것도 출력 안함
		return ;
	s_num = ft_itoa(num);
	if (num < 0)
		ft_putchar('-');
	ft_putchar_base('0', zero_num);
	if (num != 0 || flags->dot < 0) // 값이 0일 때 precision이 존재하면 값인 0이 출력 되지 않음
	{
		if (num < 0)
			ft_putstr(s_num + 1);
		else
			ft_putstr(s_num);
	}
	if (s_num)
		free(s_num);
	s_num = 0;	
}

void		print_d(int num, t_flags *flags)
{
	int zero_num;
	int blank_num;

	if (flags->dot >= 0 && flags->zero == 1) // precision이 존재하면서 zero가 켜져 있으면 (0)플래그 꺼줌
		flags->zero = 0;
	zero_num = zero_number(num, flags); // 0 출력 개수, 0flag or precision인 경우
	blank_num = blank_number(num, flags, zero_num); // 공백 출력 개수
	if (flags->minus == 1) // 왼쪽 정렬
	{
		number_output(num, zero_num, flags); // 문자 출력 
		blank_output(blank_num); // 공백 출력
	}
	else
	{
		blank_output(blank_num); // 공백 출력
		number_output(num, zero_num, flags); // 문자 출력
	}
}

void		print_p(long long pointer, t_flags *flags) // long long을 쓰는 이유는 원래 포인터로 선언된 변수는 long long으로 선언되어 있음
{
	char *p_str;
	char *zero_x;
	int p_len;
	
	zero_x = "0x";
	p_str = ft_ltoa(pointer);
	p_len = ft_strlen(p_str) + 2;
	if (flags->dot == 0 && pointer == 0) // ("%.p", NULL) -> |0x |   ("%p", NULL) -> |0x0|
		p_len--;
	if (flags->minus == 1)
	{
		ft_putstr(zero_x);
		if (flags->dot != 0 || pointer != 0)
			ft_putstr(p_str);
		if (flags->width > p_len)
			ft_putchar_base(' ', flags->width - p_len);
	}
	else
	{
		if (flags->width > p_len)
			ft_putchar_base(' ', flags->width - p_len);
		ft_putstr(zero_x);
		if (flags->dot != 0 || pointer != 0)
			ft_putstr(p_str);
	}
}

void		print_percent(t_flags *flags)
{
	int width_num;

	width_num = flags->width - 1;
	if (width_num < 0)
		width_num = 0;
	if (flags->minus == 1)
	{
		if (flags->zero == 1)
			ft_putchar_base('0', width_num);
		ft_putchar('%');
		if (flags->zero == 0)
			ft_putchar_base(' ', width_num);
	}
	else
	{
		if (flags->zero == 1)
			ft_putchar_base('0', width_num);
		else
			ft_putchar_base(' ', width_num);
		ft_putchar('%');
	}
}

int			zero_number_ui(char *s_num, t_flags *flags)
{
	int num_len;
	int zero_num;

	zero_num = 0;
	num_len = ft_strlen(s_num);
	if (flags->dot >= 0 && flags->zero == 1)
		flags->zero = 0;
	if (flags->dot > 0 && flags->dot > num_len)
		zero_num = flags->dot - num_len;
	if (flags->zero == 1 && flags->width > num_len)
		zero_num = flags->width - num_len;
	return (zero_num);
}

int			blank_number_ui(char *s_num, t_flags *flags, int zero_num, unsigned int num)
{
	int num_len;
	int blank_num;

	blank_num = 0;
	num_len = ft_strlen(s_num);
	if (flags->zero == 1)
		return (blank_num);
	if (flags->dot == 0 && num_len > 0 && num == 0)
		num_len--;
	if (flags->width > (num_len + zero_num))
		blank_num = flags->width - (num_len + zero_num);
	return (blank_num);
}

char		*is_x_or_X(char c)
{
	char *base;
	if (c == 'x')
		base = "0123456789abcdef";
	else
		base = "0123456789ABCDEF";
	return (base);
}

void		print_x(unsigned int num, t_flags *flags, char c)
{
	char *base;
	char *s_num;
	int zero_num;
	int blank_num;

	base = is_x_or_X(c);
	s_num = ft_uitoa_base(num, base, 16);
	zero_num = zero_number_ui(s_num, flags);
	blank_num = blank_number_ui(s_num, flags, zero_num, num);
	if (flags->minus == 1)
	{
		ft_putchar_base('0', zero_num);
		if (num != 0 || flags->dot != 0)
			ft_putstr(s_num);
		ft_putchar_base(' ', blank_num);
	}
	else
	{
		ft_putchar_base(' ', blank_num);
		ft_putchar_base('0', zero_num);
		if (num != 0 || flags->dot != 0)
			ft_putstr(s_num);
	}
	free(s_num);
	s_num = 0;
}

void		print_u(unsigned int num, t_flags * flags)
{
	char *base;
	char *s_num;
	int zero_num;
	int blank_num;

	base= "0123456789";
	s_num = ft_uitoa_base(num, base, 10);
	zero_num = zero_number_ui(s_num, flags);
	blank_num = blank_number_ui(s_num, flags, zero_num, num);
	if (flags->minus == 1)
	{
		ft_putchar_base('0', zero_num);
		if (num != 0 || flags->dot != 0)
			ft_putstr(s_num);
		ft_putchar_base(' ', blank_num);
	}
	else
	{
		ft_putchar_base(' ', blank_num);
		ft_putchar_base('0', zero_num);
		if (num != 0 || flags->dot != 0)
			ft_putstr(s_num);
	}
	free(s_num);
	s_num = 0;
}

void		format_specifier(va_list ap, char c, t_flags *flags) //cspdiuxX%
{
	if (c == 'd' || c == 'i')
		print_d(va_arg(ap, int), flags);
	else if (c == 'c')
		print_c(va_arg(ap, int), flags);
	else if (c == 's')
		print_s(va_arg(ap, char *), flags);
	else if (c == 'p')
		print_p(va_arg(ap, long long), flags);
	else if (c == '%')
		print_percent(flags);
	else if (c == 'x' || c == 'X')
		print_x(va_arg(ap, unsigned int), flags, c);
	else if (c == 'u')
		print_u(va_arg(ap, unsigned int), flags);
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
// 	printf("-->|%-4.x|<--\n", 0);
// 	printf("\n");
// 	ft_printf("-->|%-4.x|<--\n", 0);

// 	// system("leaks a.out > leaks_result_temp; cat leaks_result_temp | grep leaked && rm -rf leaks_result_temp");
// }