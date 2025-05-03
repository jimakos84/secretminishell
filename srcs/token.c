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
	set_single_quote_flags(*tokens);
	return (0);
}

static int  is_quote(char c)
{
	return (c == '\"' || c == '\'');
}

static int  is_special_char(char c)
{
	return (c == ' ' || c == '|' || c == '<' || c == '>');
}

static t_list *handle_token_boundary(const char *input, int *i, int *start, char quote, t_list *tokens)
{
	if (*i > *start)
		tokens = list_add_back(tokens, ft_substr(input, *start, *i - *start));
	if ((input[*i] == '>' || input[*i] == '<') && input[*i + 1] == input[*i])
	{
		tokens = list_add_back(tokens, ft_substr(input, *i, 2));
		*i += 2;
	}
	else if (input[*i] != ' ')
	{
		if (input[*i] == '|' && *i > 0 && input[*i - 1] == '|')
		{
			printf("Invalid pipe sequence\n");
			clear_tokens(tokens);
			return (NULL);
		}
		tokens = list_add_back(tokens, ft_substr(input, *i, 1));
		(*i)++;
	}
	else
		(*i)++;
	while (input[*i] == ' ' && !quote)
		(*i)++;
	*start = *i;
	return (tokens);
}

t_list *tokenize_input(const char *input)
{
	t_list	*tokens = NULL;
	int		i = 0;
	int		start = 0;
	char	quote = 0;

	while (input[i])
	{
		if (is_quote(input[i]) && !quote)
			quote = input[i];
		else if (input[i] == quote)
			quote = 0;
		if (is_special_char(input[i]) && !quote)
		{
			tokens = handle_token_boundary(input, &i, &start, quote, tokens);
			if (!tokens)
				return (NULL);
		}
		else
			i++;
	}
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
	node->in_single_quotes = 0;
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
