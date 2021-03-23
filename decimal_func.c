#include "ft_printf.h"

int			zero_number(int num, t_flags *flags, char *s_num)
{
	int		zero_num;
	int		num_len;

	zero_num = 0;
	num_len = ft_strlen(s_num);
	if (flags->zero == 1 && flags->width > num_len)
		zero_num = flags->width - num_len;
	if (num <= 0)
		num_len--;
	if (flags->dot > 0 && flags->dot > num_len)
		zero_num = flags->dot - num_len;
	return (zero_num);
}

int			blank_number(int num, t_flags *flags, int zero_num, char *s_num)
{
	int		width_num;
	int		num_len;

	width_num = 0;
	num_len = ft_strlen(s_num);
	if (num == 0 && flags->dot >= 0)
		num_len--;
	if (flags->width > (num_len + zero_num))
		width_num = flags->width - (num_len + zero_num);
	return (width_num);
}

void		number_output(int num, int zero_num, t_flags *flags, char *s_num)
{
	if (flags->dot == 0 && num == 0)
		return ;
	if (num < 0)
		ft_putchar('-');
	ft_putchar_base('0', zero_num);
	if (num != 0 || flags->dot < 0)
	{
		if (num < 0)
			ft_putstr(s_num + 1);
		else
			ft_putstr(s_num);
	}
}

void		print_d(int num, t_flags *flags)
{
	char	*s_num;
	int		zero_num;
	int		blank_num;

	s_num = ft_itoa(num);
	if (flags->dot >= 0 && flags->zero == 1)
		flags->zero = 0;
	zero_num = zero_number(num, flags, s_num);
	blank_num = blank_number(num, flags, zero_num, s_num);
	if (flags->minus == 1)
	{
		number_output(num, zero_num, flags, s_num);
		ft_putchar_base(' ', blank_num);
	}
	else
	{
		ft_putchar_base(' ', blank_num);
		number_output(num, zero_num, flags, s_num);
	}
	ft_free(s_num);
}

void		print_u(unsigned int num, t_flags *flags)
{
	char	*base;
	char	*s_num;
	int		zero_num;
	int		blank_num;

	base = "0123456789";
	s_num = ft_uitoa_base(num, base, 10);
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