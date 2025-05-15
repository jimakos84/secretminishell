/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 03:10:43 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/13 17:27:06 by tsomacha         ###   ########.fr       */
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

int	echo_error(char *msg, char *oldpwd)
{
	if (oldpwd)
		free(oldpwd);
	ft_putendl_fd(msg, 2);
	return (1);
}

void	clear_path_dirs(char **path_dirs, int i)
{
	while (path_dirs[i])
		free(path_dirs[i++]);
	free(path_dirs);
}

/*
 * Concatenates two strings and frees the first one.
 *
 * Parameters:
 * - s1: First string (will be freed).
 * - s2: Second string.
 *
 * Returns:
 * - A new string resulting from the concatenation.
 */

char	*string_build(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

int	home_not_set(char *oldpwd)
{
	printf("cd: HOME not set\n");
	free(oldpwd);
	return (1);
}
