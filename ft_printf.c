#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define conversions "cspdiuxX%"

static unsigned int g_rst;

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

void			ft_putchar(char c)
{
	write(1, &c, 1);
	g_rst += 1;
}

void			ft_putstr(char *s)
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
	flags->dot = -1; // 디폴트 -1, 0일 때 .만 .뒤에 숫자는 그대로 숫자 입력
	flags->zero = 0;
	flags->width = 0;
}

static void		flags_check(va_list ap, const char *fmt, t_flags *flags, unsigned int *i) // [flag][width][precision]각각 함수로 나눠야 함
{
	while (fmt[*i] && !(ft_strchr(conversions, fmt[*i]))) // cspdiuxX%이 안나오면 갇힐 수 있음
	{
		if (fmt[*i] == '0' && flags->dot == -1 && flags->width == 0) // 0플래그
			flags->zero = 1;
		else if (fmt[*i] == '-' && flags->dot == -1 && flags->width == 0) // -플래그
			flags->minus = 1;
		else if (flags->dot == -1 && (ft_isdigit(fmt[*i]) || fmt[*i] == '*')) // width
		{
			if (fmt[*i] == '*') 
			{
				flags->width = va_arg(ap, int);
				if (flags->width < 0)
				{
					flags->minus = 1;
					flags->width *= -1;
				}
			}
			else
				flags->width = (flags->width * 10) + (fmt[*i] - '0');	
		}
		else if (fmt[*i] == '.' || (flags->dot >= 0 && (ft_isdigit(fmt[*i]) || fmt[*i] == '*')))
		{
			if (fmt[*i] == '.')
				flags->dot = 0;
			else if (fmt[*i] == '*') // precision에 (-)플래그 못옴
				flags->dot = va_arg(ap, int);
			else
				flags->dot = (flags->dot * 10) + (fmt[*i] - '0');
		}
		(*i)++;
	}
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

static int		format_specifier(va_list ap, char c, t_flags *flags) //cspdiuxX%
{
	if (c == 'd')
		print_d(ap, flags);
	else
		return (0);
	return (1);
}

static void		ft_vsprintf(va_list ap, const char *fmt)
{
	unsigned int	i;
	t_flags			*flags;
	
	flags = (t_flags *)malloc(sizeof(t_flags) * 1);
	if (!flags)
	{
		g_rst = -1;
		return ;
	}
	i = 0;
	while (fmt[i])
	{
		if (fmt[i++] =='%')
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
	printf("|%08.6d|\n", -12345);
	//printf("|%.6d|\n", -12345);
	//printf("|%06d|\n", -12345);
	
	printf("\n");

	ft_printf("|%08.6d|\n", -12345);
	//ft_printf("|%.6d|\n", -12345);
	//ft_printf("|%06d|\n", -12345);
}