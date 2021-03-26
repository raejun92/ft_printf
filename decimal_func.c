#include "ft_printf.h"

// 기능 : 0을 출력할 개수를 구함, 리턴 : int(0의 개수)
int			zero_number(int num, t_flags *flags, char *s_num)
{
	int		zero_num;
	int		num_len;

	zero_num = 0;
	num_len = ft_strlen(s_num);
	if (flags->dot >= 0 && flags->zero == 1) // precision이 존재하고 (0)플래그가 켜져있으면
		flags->zero = 0;                     // (0)플래그 끔
	if (flags->zero == 1 && flags->width > num_len) // 0플래그인 경우(-부호를 포함한 값의 길이)
		zero_num = flags->width - num_len;
	if (num < 0) // 음수일 때 -부호 길이 빼줌
		num_len--;
	if (flags->dot > 0 && flags->dot > num_len) // precision인 경우 (-부호를 포함한 값의 길이)
		zero_num = flags->dot - num_len;
	return (zero_num);
}

// 기능 : 공백을 출력할 개수를 구함, 리턴 : int(공백의 개수)
int			blank_number(int num, t_flags *flags, int zero_num, char *s_num) 
{
	int num_len;
	int blank_num;

	blank_num = 0;
	num_len = ft_strlen(s_num);
	if (flags->zero == 1) // (0)플래그가 켜져있으면 공백대신 0으로 채움(공백개수 = 0)
		return (blank_num);
	if (flags->dot == 0 && num == 0 && num_len > 0) // precision이 0이고 값이 0이면 공백 1개 더 생성(혹시 모를 예외 처리 num_len이 음수가 되지 않게 함)
		num_len--;
	if (flags->width > (num_len + zero_num))
		blank_num = flags->width - (num_len + zero_num);
	return (blank_num);
}

// 기능 : 0과 숫자를 출력, 리턴 : void
void		number_output(int num, int zero_num, t_flags *flags, char *s_num)
{
	if (flags->dot == 0 && num == 0) // precision이 0이고 값이 0이면 아무것도 출력 안함
		return ;
	if (num < 0)
		ft_putchar('-');
	ft_putchar_base('0', zero_num);
	if (num < 0)
		ft_putstr(s_num + 1);
	else
		ft_putstr(s_num);
}

// 기능 : (-)(0)플래그, width, precision를 적용하여 숫자(int) 출력, 리턴 : void
void		print_d(int num, t_flags *flags)
{
	char	*s_num;
	int		zero_num;
	int		blank_num;

	s_num = ft_itoa(num);
	zero_num = zero_number(num, flags, s_num); // 0 출력 개수, 0플래그 or precision인 경우
	blank_num = blank_number(num, flags, zero_num, s_num); // 공백 출력 개수
	if (flags->minus == 1)
	{
		number_output(num, zero_num, flags, s_num); // 0패딩과 숫자 조합 출력
		ft_putchar_base(' ', blank_num); // 공백 출력
	}
	else
	{
		ft_putchar_base(' ', blank_num); // 공백 출력
		number_output(num, zero_num, flags, s_num); // 0패딩과 숫자 조합 출력
	}
	ft_free(s_num);
}

// 기능 : (-)(0)플래그, width, precision를 적용하여 숫자(unsigned int) 출력, 리턴 : void
void		print_u(unsigned int num, t_flags *flags)
{
	char	*base;
	char	*s_num;
	int		zero_num;
	int		blank_num;

	base = "0123456789";
	s_num = ft_uitoa_base(num, base, 10);
	zero_num = zero_number_ui(s_num, flags); // unsigned int형으로 0개수 구함
	blank_num = blank_number_ui(num, s_num, flags, zero_num); // unsigned int형으로 공백개수 구함
	if (flags->minus == 1)
	{
		ft_putchar_base('0', zero_num);
		if (num != 0 || flags->dot != 0) // 값이 0이고 precision이 0일 때 아무것도 출력 안함
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