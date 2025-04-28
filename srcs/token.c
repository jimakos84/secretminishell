#include "../includes/shell.h"

t_list *list_add_back(t_list *list, char *str);
t_list *tokenize_input(const char *input);

int extract_tokens(t_list **tokens, char *input)
{
	// Using the new tokenizer function to handle quotes and spaces
	*tokens = tokenize_input(input);
	return (0);
}

t_list *tokenize_input(const char *input)
{
	t_list *tokens = NULL;
	int i = 0, start = 0;
	char quote = 0;

	// Iterate through the input and identify tokens, handling quotes properly
	while (input[i])
	{
		// Start of a quote
		if ((input[i] == '\'' || input[i] == '\"') && !quote)
			quote = input[i];
		// End of a quote
		else if (input[i] == quote)
			quote = 0;

		// If we encounter a space, redirect symbol, or pipe, and we're not inside quotes, treat as token boundary
		if ((input[i] == ' ' || input[i] == '>' || input[i] == '<' || input[i] == '|') && !quote)
		{
			if (i > start)  // If there's a token before this space
				tokens = list_add_back(tokens, ft_substr(input, start, i - start));

			// Handle the operator or pipe
			if (input[i] != ' ')
				{
                // If we encounter a pipe and the previous token was also a pipe, it's an invalid sequence
                if (input[i] == '|' && i > 0 && input[i - 1] == '|')
                {
                    printf("Invalid pipe sequence\n");
					clear_tokens(tokens);
                    return (NULL);
                }
                tokens = list_add_back(tokens, ft_substr(input, i, 1));
            }

			// Move to next character and skip extra spaces
			i++;
			while (input[i] == ' ' && !quote)
				i++;
			start = i;
		}
		else
			i++;
	}

	// Add the last token if we have one
	if (i > start)
		tokens = list_add_back(tokens, ft_substr(input, start, i - start));

	return tokens;
}

t_list *list_add_back(t_list *list, char *str)
{
	t_list *node = malloc(sizeof(t_list)), *current;
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

t_cmd *list_add_command(t_cmd *cmds, t_cmd *node)
{
	t_cmd *current;
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
