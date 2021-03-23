#include "ft_printf.h"

void		ft_putchar(char c)
{
	write(1 , &c, 1);
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
	int	len;

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