/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:35:17 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/07 15:14:20 by dvlachos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
 * Function declaration of helper fuctions
*/
int		check_if_quoted(const char *input);
int		enclosed_in_quotes(const char *str);
void	set_single_quote_flags(t_list *tokens);
void	remove_quotes_inplace(char *str);
bool	contains_unquoted_char(char *str, char symbol);

/*
* Checks if a string is fully enclosed in single or double quotes.
*
* Parameters:
* - input: The input string.
*
* Returns:
* - 1 if the string starts and ends with the same quote type, 0 otherwise.
*/

int	check_if_quoted(const char *input)
{
	int	len;

	len = ft_strlen(input);
	if (len == 0)
		return (0);
	else if (input[0] == '\'' && input[len - 1] == '\'')
		return (1);
	else if (input[0] == '"' && input[len - 1] == '"')
		return (1);
	else
		return (0);
}

/*
* Checks if a string starts and contains a matching closing quote.
*
* Parameters:
* - str: The input string.
*
* Returns:
* - 1 if the string begins with a quote and contains a matching closing quote,
* 0 otherwise.
*/

int	enclosed_in_quotes(const char *input)
{
	int	in_single_quotes;
	int	in_double_quotes;
	int	i;

	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (input && input[i])
	{
		if (input[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		if (input[i] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		i++;
	}
	return (in_single_quotes || in_double_quotes);
}

/*
* Sets a flag in each token indicating if it is enclosed in single quotes.
*
* Parameters:
* - tokens: The linked list of token structures.
*/

void	set_single_quote_flags(t_list *tokens)
{
	t_list	*curr;
	int		len;

	curr = tokens;
	while (curr)
	{
		len = ft_strlen(curr->token);
		if (len >= 2 && curr->token[0] == '\'' && curr->token[len - 1] == '\'')
			curr->in_single_quotes = 1;
		else
			curr->in_single_quotes = 0;
		curr = curr->next;
	}
}

/*
* Removes all single and double quotes from a string in place.
*
* This function modifies the original string and does not allocate new memory.
*
* Parameters:
* - str: The string to process.
*/

void	remove_quotes_inplace(char *str)
{
	int	i;
	int	j;
	int	quote;

	i = 0;
	j = 0;
	if (!str)
		return ;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && !quote)
			quote = str[i++];
		else if (str[i] == quote)
		{
			quote = 0;
			i++;
		}
		else
			str[j++] = str[i++];
	}
	str[j] = '\0';
}

/*
* Checks if a string contains a given character outside of any quotes.
*
* Parameters:
* - str: The input string.
* - symbol: The character to search for (e.g., '|').
*
* Returns:
* - true if the character is found unquoted, false otherwise.
*/

bool	contains_unquoted_char(char *str, char symbol)
{
	bool	in_single;
	bool	in_double;
	int		i;

	i = 0;
	in_single = false;
	in_double = false;
	if (!str)
		return (false);
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		else if (str[i] == symbol && !in_single && !in_double)
			return (true);
		i++;
	}
	return (false);
}
