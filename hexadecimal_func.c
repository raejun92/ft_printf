#include "ft_printf.h"

void		print_p(long long p, t_flags *flags)
{
	char	*p_str;
	char	*zero_x;
	int		p_len;

	zero_x = "0x";
	p_str = ft_ltoa(p);
	p_len = ft_strlen(p_str) + 2;
	if (flags->dot == 0 && p == 0)
		p_len--;
	if (flags->minus == 1)
	{
		ft_putstr(zero_x);
		if (flags->dot != 0 || p != 0)
			ft_putstr(p_str);
		if (flags->width > p_len)
			ft_putchar_base(' ', flags->width - p_len);
	}
	else
	{
		if (flags->width > p_len)
			ft_putchar_base(' ', flags->width - p_len);
		ft_putstr(zero_x);
		if (flags->dot != 0 || p != 0)
			ft_putstr(p_str);
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

int			blank_number_ui(unsigned int num, char *s_num, t_flags *flags, int zero_num)
{
	int num_len;
	int blank_num;

	blank_num = 0;
	num_len = ft_strlen(s_num);
	if (flags->zero == 1)
		return (blank_num);
	if (flags->dot == 0 && num == 0 && num_len > 0)
		num_len--;
	if (flags->width > (num_len + zero_num))
		blank_num = flags->width - (num_len + zero_num);
	return (blank_num);
}

void		print_x(unsigned int num, t_flags *flags, char *base)
{
	char	*s_num;
	int		zero_num;
	int		blank_num;

	s_num = ft_uitoa_base(num, base, 16);
	zero_num = zero_number_ui(s_num, flags);
	blank_num = blank_number_ui(num, s_num, flags, zero_num);
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
	ft_free(s_num);
}