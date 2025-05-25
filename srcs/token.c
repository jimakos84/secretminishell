/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:11:59 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/25 13:18:04 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
 * Function declaration of helper fuctions
 */
t_list	*list_add_back(t_list *list, char *str);
t_list	*tokenize_input(const char *input);
int		ft_isquoted(const char *str, int n);

/*
 * Extracts tokens from the input string and sets quote-related flags.
 * - Tokenizes the input string into a linked list of tokens.
 * - Returns an error if tokenization fails.
 * - Sets flags on tokens to mark those enclosed in single quotes.
 *
 * Parameters:
 * - tokens: A pointer to the token list to populate.
 * - input: The input string to tokenize.
 *
 * Returns:
 * - 0 on success, or 1 if tokenization fails.
 */

int	extract_tokens(t_list **tokens, char *input)
{
	*tokens = tokenize_input(input);
	if (!*tokens)
		return (1);
	set_single_quote_flags(*tokens);
	return (0);
}

/*
 * Tokenizes the full input string into a linked list of token nodes.
 * - Iterates over the input string.
 * - Uses `handle_token` to extract one token at a time.
 * - Adds each token to a linked list.
 *
 * Parameters:
 * - input: The full command line string.
 *
 * Returns:
 * - A linked list of tokens, or NULL if allocation fails.
 */

t_list	*tokenize_input(const char *input)
{
	char	*token;
	int		i;
	t_list	*tokens;

	token = NULL;
	tokens = NULL;
	i = 0;
	while (input && input[i])
	{
		while (input && input[i] && ft_isspace(input[i]))
			i++;
		if (input[i] == '\0')
			break ;
		token = handle_token(input, &i);
		tokens = list_add_back(tokens, token);
		free(token);
	}
	return (tokens);
}

/*
 * Adding a new token node to the end of a linked list of tokens.
 * - Allocates a new list node and duplicates the token string.
 * - Initializes quote-related flags to 0.
 * - Adds the new node to the end of the list.
 *
 * Parameters:
 * - list: The existing list of tokens.
 * - str: The token string to add.
 *
 * Returns:
 * - The updated list of tokens, or NULL on allocation failure.
 */

t_list	*list_add_back(t_list *list, char *str)
{
	t_list	*current;
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
	{
		perror("node initializing fails !");
		return (NULL);
	}
	if (!str)
	{
		free(node);
		return (NULL);
	}
	node->next = NULL;
	node->token = ft_strdup(str);
	node->in_single_quotes = 0;
	node->prev_heredoc = 0;
	if (!list)
		return (node);
	current = list;
	while (current && current->next)
		current = current->next;
	current->next = node;
	return (list);
}

/*
 * Adding a command node to the end of a linked list of commands.
 *
 * Parameters:
 * - cmds: The existing list of commands.
 * - node: The command node to append.
 *
 * Returns:
 * - The updated command list.
 */

t_cmd	*list_add_command(t_cmd *cmds, t_cmd *node)
{
	t_cmd	*current;

	if (!node)
		return (cmds);
	if (!cmds)
		return (node);
	current = cmds;
	while (current && current->next)
		current = current->next;
	current->next = node;
	return (cmds);
}
/*
 * Determines whether a character at a given index is inside quotes.
 * - Iterates through the string up to the given index.
 * - Tracks open and close states for single and double quotes.
 * - Returns true if the character at index `n` is inside quotes.
 *
 * Parameters:
 * - str: The string to check.
 * - n: The index to evaluate.
 *
 * Returns:
 * - 1 if inside quotes, 0 otherwise.
 */

int	ft_isquoted(const char *str, int n)
{
	int	in_single;
	int	in_double;
	int	i;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (str && str[i] && i != n)
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single || in_double);
}
