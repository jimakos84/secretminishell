/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:13:47 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/25 14:09:11 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
 * Function declaration of helper fuctions
*/
int		count_args(t_list *tokens);
int		ft_is_in_single(const char *str, int n);
void	process_token_expansion(t_list *current, t_shell *mini);
void	handle_dollar(t_list *list, t_shell *mini);

/*
* Counts the number of tokens in a linked list.
*
* Parameters:
* - tokens: Pointer to the head of the token list.
*
* Returns:
* - The number of tokens in the list.
*/

int	count_args(t_list *tokens)
{
	int		count;
	t_list	*current;

	current = tokens;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/*
* Checks if a given position in a string is inside single quotes.
*
* Parameters:
* - str: The input string.
* - n: The index to check.
*
* Returns:
* - 1 if inside single quotes at position n, 0 otherwise.
*/

int	ft_is_in_single(const char *str, int n)
{
	int	in_single;
	int	i;

	in_single = 0;
	i = 0;
	while (str && str[i] && i != n)
	{
		if (str[i] == '\'' && !in_single)
			in_single = !in_single;
		else if (str[i] == '\'' && in_single)
			in_single = !in_single;
		i++;
	}
	return (in_single);
}

/*
* Expands a single token in the list if it is not within single quotes.
*
* Parameters:
* - current: The current token node.
* - mini: Pointer to shell context.
*/

void	process_token_expansion(t_list *current, t_shell *mini)
{
	char	*new_token;
	char	*old_token;

	if (current->in_single_quotes)
		return ;
	old_token = current->token;
	new_token = expand_token(old_token, mini);
	if (!new_token || new_token[0] == '\0')
	{
		if (new_token)
			free(new_token);
		current->token = NULL;
		free(old_token);
		return ;
	}
	current->token = ft_strdup(new_token);
	free(old_token);
	free(new_token);
}

/*
* Traverses the token list and expands variables prefixed by '$'.
*
* Parameters:
* - list: The list of input tokens.
* - mini: Pointer to shell context.
*/

void	handle_dollar(t_list *list, t_shell *mini)
{
	t_list	*current;
	char	*str;

	current = list;
	str = NULL;
	while (current)
	{
		str = ft_strchr(current->token, '$');
		if (str && !current->prev_heredoc)
			process_token_expansion(current, mini);
		current = current->next;
	}
}
