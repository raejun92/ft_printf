#include "ft_printf.h"

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
	
	if (!s)
		s = "(null)";
	s_len = ft_strlen(s);
	if (flags->dot >= 0 && flags->dot < s_len)
		s_len = flags->dot;
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