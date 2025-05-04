/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 05:39:43 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/04 06:07:51 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
 * Function declaration of helper fuctions
 */
static char	*input_preprocess(char **input);
static int	check_syntax(char *input, char redirect_char);
static char	*remove_comments(char *input);

/*
 * Validates the user input by preprocessing and checking for
 * basic syntax errors.
 * - Removes comments and trims whitespace from the input.
 * - Checks for invalid usage of '>', '<', and '|' characters.
 * - If a syntax error is found, it returns a `syntax_error`.
 *
 * Parameters:
 * - input: A pointer to the input string (modifiable).
 *
 * Returns:
 * - 0 if the input is valid, or a non-zero value if a syntax error
 * is detected.
 */
int	input_validate(char **input)
{
	char	*trimmed;

	trimmed = input_preprocess(input);
	if (check_syntax(trimmed, '>'))
		return (syntax_error(trimmed, "'newline'", 2));
	if (check_syntax(trimmed, '<'))
		return (syntax_error(trimmed, "'newline'", 2));
	if (check_syntax(trimmed, '|'))
		return (syntax_error(trimmed, "'|'", 2));
	free(trimmed);
	return (0);
}

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
static char	*remove_comments(char *input)
{
	char	*comment_start;
	char	*clean_input;

	comment_start = ft_strchr(input, '#');
	if (comment_start)
		clean_input = ft_strnmdup(input, 0, comment_start - input);
	else
		clean_input = ft_strdup(input);
	return (clean_input);
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
static char	*input_preprocess(char **input)
{
	char	*no_comments;
	char	*trimmed;

	no_comments = remove_comments(*input);
	trimmed = ft_strtrim(no_comments, ISSPACE3);
	free(no_comments);
	return (trimmed);
}

/*
 * Checks for invalid syntax patterns related to redirection or pipes.
 * - Scans the input string for a specific redirection or pipe character.
 * - Verifies that it is not followed by invalid patterns like nothing,
 * space only, or repeated operator.
 *
 * Parameters:
 * - input: The cleaned-up command line string.
 * - redirect_char: The character to check ('<', '>', or '|').
 *
 * Returns:
 * 1 if a syntax error is detected, 0 otherwise.
 */
static int	check_syntax(char *input, char redirect_char)
{
	int	i;

	i = 0;
	while (input && input[i])
	{
		if (input[i] == redirect_char)
		{
			i++;
			if (input[i] == redirect_char)
				i++;
			while (input[i] && ft_isspace(input[i]))
				i++;
			if (!input[i] || input[i] == redirect_char)
				return (1);
		}
		else
			i++;
	}
	return (0);
}
