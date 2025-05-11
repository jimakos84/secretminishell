/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 03:10:43 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/09 03:11:13 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Validates whether a given name is a valid identifier for a specified length.
*
* Rules:
* - The first character must be a letter or underscore.
* - Remaining characters must be alphanumeric or underscores.
*
* Parameters:
* - name: The string to validate.
* - len: The number of characters to validate.
*
* Returns:
* - 1 if the name is a valid identifier, 0 otherwise.
*/

int	is_valid_identifier_len(const char *name, int len)
{
	int	i;

	if (len == 0 || !(ft_isalpha(name[0]) || name[0] == '_'))
		return (0);
	i = 1;
	while (i < len)
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

/*
* Prints an error message to stderr and returns 1.
*
* Parameters:
* - msg: The error message to display.
*
* Returns:
* - Always returns 1.
*/

int	echo_error(char *msg)
{
	ft_putendl_fd(msg, 2);
	return (1);
}
