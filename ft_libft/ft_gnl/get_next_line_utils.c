/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 12:23:19 by tsomacha          #+#    #+#             */
/*   Updated: 2024/12/05 19:33:40 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

ssize_t	ft_haschar(char *str, int c)
{
	size_t	count;

	count = 0;
	while (str && str[count])
	{
		if (str[count] == (char)c)
			return (count);
		count++;
	}
	return (-1);
}

char	*ft_extract_line(char *str, int c)
{
	ssize_t	count;
	ssize_t	index;
	char	*new_line;

	count = 0;
	index = ft_haschar(str, c);
	if (index < 0)
		return (NULL);
	new_line = malloc((index + 2) * sizeof(char));
	if (!new_line)
		return (NULL);
	while (str && str[count] && count < index)
	{
		new_line[count] = str[count];
		count++;
	}
	new_line[count] = '\n';
	new_line[count + 1] = '\0';
	return (new_line);
}
