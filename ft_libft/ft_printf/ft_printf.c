/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 20:58:52 by tsomacha          #+#    #+#             */
/*   Updated: 2024/11/28 13:43:35 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_handleexception(char percentage, char flag)
{
	int	len;
	int	res;

	len = ft_printchar(percentage);
	res = ft_printchar(flag);
	if (len < 0 || res < 0)
		return (-1);
	return (res + len);
}

static int	ft_formatconversion(char flag, va_list args)
{
	if (flag == 'c')
		return (ft_printchar(va_arg(args, int)));
	else if (flag == 's')
		return (ft_printstr(va_arg(args, char *)));
	else if (flag == 'p')
		return (ft_printpointer(va_arg(args, uintptr_t)));
	else if (flag == 'i' || flag == 'd')
		return (ft_printnbr_base(va_arg(args, int), 10, 0));
	else if (flag == 'u')
		return (ft_printnbr_base(va_arg(args, unsigned), 10, 0));
	else if (flag == 'x')
		return (ft_printnbr_base(va_arg(args, unsigned), 16, 0));
	else if (flag == 'X')
		return (ft_printnbr_base(va_arg(args, unsigned), 16, 1));
	else if (flag == '%')
		return (ft_printchar('%'));
	else
		return (ft_handleexception('%', flag));
}

static int	ft_handleformat(const char *format, va_list args, int *ptr, int *e)
{
	int	res;

	res = 0;
	if (format[*ptr] == '%' && format[*ptr + 1] != '\0')
	{
		res = ft_formatconversion(format[++(*ptr)], args);
		if (res < 0)
			*e = -1;
		return (res);
	}
	else if (format[*ptr] == '%' && format[*ptr + 1] == '\0')
	{
		*e = -1;
		return (-1);
	}
	else
	{
		res = ft_printchar(format[*ptr]);
		if (res < 0)
			*e = -1;
		return (res);
	}
	return (res);
}

int	ft_printf(const char *format, ...)
{
	int		printed;
	int		count;
	int		error;
	va_list	args;

	va_start(args, format);
	printed = 0;
	count = 0;
	error = 0;
	while (format[count] && error != -1)
	{
		error = 0;
		printed += ft_handleformat(format, args, &count, &error);
		count++;
	}
	va_end(args);
	if (error < 0)
		printed = error;
	return (printed);
}
