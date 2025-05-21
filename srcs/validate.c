/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 05:39:43 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/15 05:53:42 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Function declaration of helper fuctions
*/
static int	check_syntax(char *input, char redirect_char, t_initenv *env);
static int	check_pipe_char(char *input, char redirect_char, t_initenv *env);
static int	check_special_char(char *input, char *charset);
static int	check_special_occurance(char *input);

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
int	input_validate(char **input, t_initenv *env)
{
	char	*trimmed;

	trimmed = input_preprocess(input);
	if (enclosed_in_quotes(trimmed))
		return (syntax_error(trimmed, 2));
	if (check_special_char(trimmed, SPECIALCHARS))
		return (syntax_error(trimmed, 2));
	if (check_special_occurance(trimmed))
		return (syntax_error(trimmed, 2));
	if (check_pipe_char(trimmed, '|', env))
		return (syntax_error(trimmed, 2));
	if (check_syntax(trimmed, '>', env) == 2)
		return (2);
	if (check_syntax(trimmed, '>', env) == 1)
		return (syntax_error(trimmed, 2));
	if (check_syntax(trimmed, '<', env) == 2)
		return (2);
	if (check_syntax(trimmed, '<', env) == 1)
		return (syntax_error(trimmed, 2));
	free(trimmed);
	return (0);
}

/*
* Checks for invalid syntax patterns related to redirection or pipes.
* - Scans the input string for a specific redirection or pipe character.
* - Verifies that it is not followed by invalid patterns like nothing,
* space only, or repeated operator.
*
* Parameters:
* - input: The cleaned-up command line string.
* - redirect_char: The character to check ('<', '>').
*
* Returns:
* 1 if a syntax error is detected, 0 otherwise.
*/

static int	check_syntax(char *input, char redirect_char, t_initenv *env)
{
	int	i;

	i = 0;
	while (input && input[i])
	{
		if (input[i] == redirect_char && !ft_isquoted(input, i))
		{
			i++;
			if (input[i] == redirect_char && !ft_isquoted(input, i))
				i++;
			while (input[i] && ft_isspace(input[i]))
				i++;
			if (input[i] && input[i] == redirect_char && !ft_isquoted(input, i))
				return (1);
			if ((!input[i] || input[i] == redirect_char))
				return (1);
			if (input[i] == '$' && check_expansion(input, env, i))
				return (2);
		}
		else
			i++;
	}
	return (0);
}

/*
* Checks for invalid syntax patterns related to redirection or pipes.
* - Scans the input string for a specific redirection or pipe character.
* - Verifies that it is not followed by invalid patterns like nothing,
* space only, or repeated operator.
*
* Parameters:
* - input: The cleaned-up command line string.
* - redirect_char: The character to check '|'
*
* Returns:
* 1 if a syntax error is detected, 0 otherwise.
*/

static int	check_pipe_char(char *input, char redirect_char, t_initenv *initenv)
{
	int	i;

	i = 0;
	if (input && input[0] == redirect_char && !ft_isquoted(input, 0))
		return (1);
	while (input && input[i])
	{
		if (input[i] == redirect_char && !ft_isquoted(input, i))
		{
			i++;
			if (input[i] == redirect_char && !ft_isquoted(input, i))
				return (1);
			i++;
			while (input[i] && ft_isspace(input[i]))
				i++;
			if ((input[i] == redirect_char && !ft_isquoted(input, i))
				|| (input[i] == '$' && !ft_isquoted(input, i)
					&& check_expansion2(input, initenv, i)))
				return (1);
		}
		else
			i++;
	}
	return (0);
}

/*
* Checks if the input string contains any unquoted special characters.
* - Scans the input for any character from the specified `charset`.
* - Ignores characters that appear inside quotes (single or double).
* - Returns 1 as soon as an unquoted special character is found.
* - The charset typically includes shell metacharacters, such as:
*   ()[]{}&;\\ (defined by the SPECIALCHARS macro).
*
* Parameters:
* - input: The input string to scan.
* - charset: A string containing special characters to look for.
*
* Returns:
* - 1 if an unquoted special character is found, 0 otherwise.
*/

static int	check_special_char(char *input, char *charset)
{
	int		i;
	char	*str;

	i = 0;
	while (charset && charset[i])
	{
		str = input;
		while (str)
		{
			str = ft_strchr(str, charset[i]);
			if (str)
			{
				if (!ft_isquoted(input, str - input))
					return (1);
				str++;
			}
		}
		i++;
	}
	return (0);
}

/*
* Checks for invalid sequences of redirection and pipe characters in the input.
*
* This function looks for specific syntax errors involving redirection:
* - A '<' followed by optional whitespace and then a '|' (e.g., `<   |`)
* - A '>' followed by optional whitespace and then either a '<' or '|'
* (e.g., `>   <` or `>   |`)
*
* Such sequences are generally considered invalid in shell parsing logic.
*
* Parameters:
* - input: The command line input string to validate.
*
* Returns:
* - 1 if an invalid special character sequence is found, 0 otherwise.
*/

static int	check_special_occurance(char *input)
{
	int	len;

	len = ft_strlen(input);
	if (len == 1)
		if (validation_for_onechar(input))
			return (1);
	if (len == 2)
		if (validation_for_twochar(input))
			return (1);
	if (len == 3)
		if (validation_for_threechar(input))
			return (1);
	if (input[len - 1] == '|')
		return (1);
	return (0);
}
