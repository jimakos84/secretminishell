/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 04:14:46 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/07 09:28:11 by dvlachos         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../includes/shell.h"

/*
 * Fnction declaration for helper funtions
*/
char	*handle_token(const char *input, int *index);

/*
 * Extracts a single token from the input string starting at the given index.
 * - Reads characters until it encounters unquoted whitespace.
 * - Returns a newly allocated substring as a token.
 * - Updates the index to the position after the extracted token.
 *
 * Parameters:
 * - input: The full input string.
 * - index: Pointer to the current index in the input string.
 *
 * Returns:
 * - A new string containing the token.
 * - Returns NULL in the evenet inpt is also NULL.
 */

char	*handle_token(const char *input, int *index)
{
	int	i;
	int	start;

	i = *index;
	start = i;
	if (is_redir_or_pipe(input[i]) && !ft_isquoted(input, i))
	{
		if (input[i] == input[i + 1])
			i += 2;
		else
			i += 1;
		*index = i;
		return (ft_substr(input, start, i - start));
	}
	while (input[i])
	{
		if (ft_isspace(input[i]) && !ft_isquoted(input, i))
			break ;
		if (is_redir_or_pipe(input[i]) && !ft_isquoted(input, i))
			break ;
		i++;
	}
	*index = i;
	return (ft_substr(input, start, i - start));
}
