#include "ft_printf.h"

void		is_flag(char fmt, t_flags *flags)
{
	if (fmt == '0' && flags->width == 0 && flags->dot == -1)
		flags->zero = 1;
	else if (fmt == '-' && flags->width == 0 && flags->dot == -1)
		flags->minus = 1;
}

void		is_width(char fmt, t_flags *flags, va_list ap)
{
	if (flags->dot == -1 && (ft_isdigit(fmt) || fmt == '*'))
	{
		if (fmt == '*')
		{
			flags->width = va_arg(ap, int);
			if (flags->width < 0)
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
