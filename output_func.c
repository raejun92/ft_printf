#include "ft_printf.h"

// 기능 : 문자 출력, 리턴 : void
void		ft_putchar(char c)
{
	write(1 , &c, 1);
	g_rst += 1;
}

// 기능 : 문자 len번 출력, 리턴 : void 
void		ft_putchar_base(char c, int len)
{
	g_rst += len;
	while (len--)
		write(1, &c, 1);
}

// 기능 : 문자열 출력, 리턴 : void
void		ft_putstr(char *s)
{
	int	len;

	if (!s)
		return ;
	len = ft_strlen(s);
	write(1, s, len);
	g_rst += len;
}

// 기능 : len길이 만큼 문자열 출력, 리턴 : void
void		ft_putstr_base(char *s, int len)
{
	if (!s)
		return ;
	write(1, s, len);
	g_rst += len;
}