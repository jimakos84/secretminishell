/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pointer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 13:14:14 by tsomacha          #+#    #+#             */
/*   Updated: 2024/11/28 11:41:51 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_pointer(uintptr_t nbr)
{
	int	len;
	int	res;

	len = 0;
	res = 0;
	if (nbr < 16)
	{
		res = ft_printdigit(nbr, 0);
		if (res < 0)
			return (-1);
		len += res;
	}
	else
	{
		res = ft_pointer((nbr / 16));
		len = ft_printdigit((nbr % 16), 0);
		if (res < 0 || len < 0)
			return (-1);
		len += res;
	}
	return (len);
}

int	ft_printpointer(uintptr_t nbr)
{
	int	len;
	int	res;

	len = 0;
	res = 0;
	if (nbr == 0)
	{
		res = ft_printstr("(nil)");
		if (res < 0)
			return (-1);
		len += res;
	}
	else
	{
		res = ft_printstr("0x");
		len = ft_pointer(nbr);
		if (res < 0 || len < 0)
			return (-1);
		len += res;
	}
	return (len);
}
