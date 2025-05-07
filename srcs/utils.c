/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:16:31 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/07 11:37:36 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/**
 * Function declaration
*/
int		ft_strnmcpy(char **dest, char *src, int n, int m);
int		ft_arraylen(char **envp);
int		ft_isempty(char *str);
int		ft_isspace(int c);
char	*ft_strnmdup(char const *src, int n, int m);

/*
* Copies a substring from `src` between indices `n` and `m` into
* a newly allocated string.
*
* Parameters:
* - dest: A pointer to the destination string pointer (will be allocated).
* - src: The source string.
* - n: The starting index (inclusive).
* - m: The ending index (exclusive).
*
* Returns:
* - The number of characters copied on success, or -1 on error.
*/

int	ft_strnmcpy(char **dest, char *src, int n, int m)
{
	int	i;

	i = 0;
	if (n >= m || !src || !dest)
		return (-1);
	*dest = malloc(sizeof(char) * (m - n + 1));
	if (!*dest)
		return (-1);
	while ((i + n) < m && src[i + n])
	{
		(*dest)[i] = src[i + n];
		i++;
	}
	(*dest)[i] = '\0';
	return (i);
}

/*
* Calculates the number of strings in a NULL-terminated string array.
*
* Parameters:
* - envp: The string array (e.g., environment variables).
*
* Returns:
* - The number of elements in the array.
*/

int	ft_arraylen(char **envp)
{
	int		len;

	len = 0;
	while (envp && envp[len])
		len++;
	return (len);
}

/*
* Checks if a string is empty or consists only of whitespace characters.
*
* Parameters:
* - str: The input string.
*
* Returns:
* - 1 if the string is empty or whitespace-only, 0 otherwise.
*/

int	ft_isempty(char *str)
{
	while (*str)
	{
		if (!ft_isspace(*str))
			return (0);
		str++;
	}
	return (1);
}

/*
* Determines if a character is a whitespace character.
*
* Recognized whitespace: space, tab, newline, carriage return, vertical tab.
*
* Parameters:
* - c: The character to check.
*
* Returns:
* - 1 if the character is whitespace, 0 otherwise.
*/

int	ft_isspace(int c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v')
		return (1);
	else
		return (0);
}

/*
* Duplicates a substring from `src` between indices `n` and `m` into
* a new string.
*
* Parameters:
* - src: The source string.
* - n: The starting index (inclusive).
* - m: The ending index (exclusive).
*
* Returns:
* - A newly allocated substring, or NULL on failure.
*/

char	*ft_strnmdup(char const *src, int n, int m)
{
	int		i;
	char	*dest;

	i = 0;
	if (!src || m == 0)
		return (NULL);
	dest = malloc(sizeof(char) * ((m - n) + 1));
	if (!dest)
		return (NULL);
	while (src && src[i + n] && (i + n) < m)
	{
		dest[i] = src[i + n];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
