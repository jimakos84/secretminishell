/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:32:01 by tsomacha          #+#    #+#             */
/*   Updated: 2024/11/24 16:16:47 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_handle_negative(long nbr, int base, int set_case)
{
	int	len;
	int	res;

	len = ft_printchar('-');
	res = ft_printnbr_base((0 - nbr), base, set_case);
	if (len < 0 || res < 0)
		return (-1);
	return (len + res);
}

int	ft_handle_recursive(long nbr, int base, int set_case, int *len)
{
	int	res;

	res = ft_printnbr_base(nbr / base, base, set_case);
	if (res < 0)
		return (-1);
	*len += res;
	return (ft_printdigit(nbr % base, set_case));
}

int	ft_printnbr_base(long nbr, int base, int set_case)
{
	int	len;
	int	res;

	len = 0;
	res = 0;
	if (nbr < 0)
		return (ft_handle_negative(nbr, base, set_case));
	if (nbr >= base)
	{
		res = ft_handle_recursive(nbr, base, set_case, &len);
		if (res < 0)
			return (-1);
		len += res;
	}
	else
	{
		res = ft_printdigit(nbr, set_case);
		if (res < 0)
			return (-1);
		len += res;
	}
	return (len);
}
