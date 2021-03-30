#include "ft_printf.h"

// 기능 : 숫자n을 base진수로 나누어 길이 파악, 리턴 : int(숫자 길이)
static int	ft_number_len_base(long long n, int base)
{
	int	i;

	i = 0;
	if (n <= 0)
		i += 1;
	while (n)
	{
		n /= base;
		i++;
	}
	return (i);
}

// 기능 : int형 숫자를 char형 숫자로 바꿈, 리턴 : char *(숫자)
char		*ft_itoa(int n)
{
	char		*str;
	long long	ln;
	int			len;
	int			i;

	ln = (long long)n;
	len = ft_number_len_base(ln, 10);
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
		ln /= 10;
	}
	return (str);
}

// long long형 숫자를 16진수 char형 숫자로 바꿈, 리턴 : char *(숫자)
char		*ft_ltoa(long long n)
{
	char	*str;
	char	*base;
	int		len;
	int		i;

	base = "0123456789abcdef";
	if (n < 0) // 포인터값은 양수만 들어옴
		return (0);
	len = ft_number_len_base(n, 16);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (0);
	str[len--] = '\0';
	i = 0;
	while (i <= len)
	{
		str[len--] = base[n % 16];
		n /= 16;
	}
	return (str);
}

// 기능 : unsinged int형 숫자를 base_num진수로 나눠 base문자로 만듦, 리턴 : char *(base로 구성된 숫자ㅌㅈ)
char		*ft_uitoa_base(unsigned int n, char *base, int base_num)
{
	unsigned int	n_tmp;
	int				len;
	char			*str;

	n_tmp = n;
	len = 0;
	if (n_tmp == 0)
		len++;
	while (n_tmp)
	{
		n_tmp /= base_num;
		len++;
	}
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (0);
	str[len--] = '\0';
	while (len >= 0)
	{
		str[len--] = base[n % base_num];
		n /= base_num;
	}
	return (str);
}