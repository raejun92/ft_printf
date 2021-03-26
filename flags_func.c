#include "ft_printf.h"

// 기능 : (-)플래그 또는 (0)플래그가 존재하는가, 리턴 : void
void		is_flag(char fmt, t_flags *flags)
{
	if (fmt == '0' && flags->width == 0 && flags->dot == -1)
		flags->zero = 1;
	else if (fmt == '-' && flags->width == 0 && flags->dot == -1)
		flags->minus = 1;
}

// 기능 : width가 존재하는가, 리턴 : void
void		is_width(char fmt, t_flags *flags, va_list ap)
{
	if (flags->dot == -1 && (ft_isdigit(fmt) || fmt == '*'))
	{
		if (fmt == '*')
		{
			flags->width = va_arg(ap, int);
			if (flags->width < 0) // *이 음수이면 (-)플래그가 켜지고 *은 양수로 바꿈
			{
				flags->minus = 1;
				flags->width *= -1;
			}
		}
		else
			flags->width = (flags->width * 10) + (fmt - '0');
	}
}

// 기능 : precision이 존재하는가, 리턴 : void
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
