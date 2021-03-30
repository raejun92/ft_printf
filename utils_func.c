#include "ft_printf.h"

// 기능 : malloc할당 해제, 리턴 : void
void		ft_free(char **s)
{
	free(*s);
	*s = 0;
}

// 기능 : 문자열 길이 출력, 리턴 : size_t(문자열 길이)
size_t		ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

// 기능 : 숫자인지 판별, 리턴 : int(숫자면 1 아니면 0)
int			ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

// 기능 : s문자열에 c문자가 있는지 확인, 리턴 : int(c문자가 s문자열에 있으면 1 없으면 0)
int		ft_strchr(const char *s, int c)
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