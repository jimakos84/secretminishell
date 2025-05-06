/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:22:33 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/06 10:28:41 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Function declaration of helper fuctions
*/
char	*input_preprocess(char **input);
char	*remove_comments(char *input);

/*
* Removes shell-style comments from the input string.
* A comment is considered to start at the first '#' character
* and continues to the end of the line.
* If no comment is found, the original string is duplicated.
*
* Parameters:
* - input: The input command line string.
*
* Returns:
* - A newly allocated string with the comment (if any) removed.
*/

char	*remove_comments(char *input)
{
	int		i;

	if (!input)
		return (NULL);
	i = 0;
	while (input[i])
	{
		if (input[i] == '#' && !ft_isquoted(input, i))
			break ;
		i++;
	}
	return (ft_strnmdup(input, 0, i));
}

/*
* Preprocesses the input string by removing comments and trimming
* whitespace (defined as ISSPACES3 in the header file)
* - Removes comments using `remove_comments`.
* - Trims leading and trailing whitespace using `ft_strtrim`.
*
* Parameters:
* - input: A pointer to the original input string.
*
* Returns:
* - A newly allocated, cleaned-up version of the input.
*/

char	*input_preprocess(char **input)
{
	char	*no_comments;
	char	*trimmed;

	no_comments = remove_comments(*input);
	trimmed = ft_strtrim(no_comments, ISSPACE3);
	free(no_comments);
	return (trimmed);
}
