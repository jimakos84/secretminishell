/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:42:28 by dvlachos          #+#    #+#             */
/*   Updated: 2025/04/29 12:52:31 by dvlachos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

t_list	*list_add_back(t_list *list, char *str);
t_list	*tokenize_input(const char *input);

int	extract_tokens(t_list **tokens, char *input)
{
	*tokens = tokenize_input(input);
	return (0);
}

t_list	*tokenize_input(const char *input)
{
	t_list	*tokens = NULL;
	int		i = 0;
	int		start = 0;
	char	quote = 0;

	while (input[i])
	{
		// Start quote tracking
		if ((input[i] == '\'' || input[i] == '\"') && !quote)
			quote = input[i];
		else if (input[i] == quote)
			quote = 0;

		// If we're not inside quotes and hit a special character
		if ((input[i] == ' ' || input[i] == '|' || input[i] == '<' || input[i] == '>') && !quote)
		{
			// Add the word/token before the special character
			if (i > start)
				tokens = list_add_back(tokens, ft_substr(input, start, i - start));

			// Handle '>>' and '<<'
			if ((input[i] == '>' || input[i] == '<') && input[i + 1] == input[i])
			{
				tokens = list_add_back(tokens, ft_substr(input, i, 2));
				i += 2;
			}
			// Handle single '|', '<', '>'
			else if (input[i] != ' ')
			{
				if (input[i] == '|' && i > 0 && input[i - 1] == '|')
				{
					printf("Invalid pipe sequence\n");
					clear_tokens(tokens);
					return (NULL);
				}
				tokens = list_add_back(tokens, ft_substr(input, i, 1));
				i++;
			}
			else
				i++; // Skip space

			// Skip spaces after a special character
			while (input[i] == ' ' && !quote)
				i++;
			start = i; // update starting point for the next token
		}
		else
			i++;
	}

	// Add the final token if there's anything left
	if (i > start)
		tokens = list_add_back(tokens, ft_substr(input, start, i - start));
	return (tokens);
}


t_list	*list_add_back(t_list *list, char *str)
{
	t_list	*current;
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->next = NULL;
	node->token = ft_strdup(str);
	if (!list)
		return (node);
	current = list;
	while (current && current->next)
		current = current->next;
	current->next = node;
	return (list);
}

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
