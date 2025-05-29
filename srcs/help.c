/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:13:11 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/29 08:45:58 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Calculates the length of a linked list of environment variables.
*
* Parameters:
* - env: Pointer to the head of the environment list.
*
* Returns:
* - The number of elements in the list.
*/

int	ft_lst_len(t_env *env)
{
	t_env	*temp;
	int		len;

	len = 0;
	temp = env;
	while (temp)
	{
		len++;
		temp = temp->next;
	}
	return (len);
}

/*
* Checks for an invalid pipe sequence: two consecutive unquoted pipe characters.
*
* Parameters:
* - current: The current token node.
*
* Returns:
* - true if there are two consecutive unquoted pipes, false otherwise.
*/

bool	is_invalid_pipe_sequence(t_list *current)
{
	if (!current || !current->next)
		return (false);
	if (contains_unquoted_char(current->token, '|')
		&& contains_unquoted_char(current->next->token, '|'))
		return (true);
	return (false);
}

/*
* Determines whether a token string represents a redirection operator.
*
* Valid tokens: ">", ">>", "<", "<<"
*
* Parameters:
* - token: The token string to check.
*
* Returns:
* - 1 if the token is a redirection operator, 0 otherwise.
*/

int	is_redirection_token(char *token)
{
	if (!token)
		return (0);
	if (ft_strcmp(token, ">") == 0)
		return (1);
	if (ft_strcmp(token, ">>") == 0)
		return (1);
	if (ft_strcmp(token, "<") == 0)
		return (1);
	if (ft_strcmp(token, "<<") == 0)
		return (1);
	return (0);
}
/*
* Creates a new redirection node with the given type and filename.
*
* Quotes are removed from the filename during initialization.
*
* Parameters:
* - type: The redirection type (e.g., OPRD_CMD, IPRD_CMD).
* - filename: The target filename for the redirection.
*
* Returns:
* - A pointer to the new t_redir node, or NULL on failure.
*/

t_redir	*create_redir_node(int type, const char *filename)
{
	t_redir	*new;

	if (!filename)
		return (NULL);
	new = ft_calloc(1, sizeof(t_redir));
	if (!new)
		return (NULL);
	new->type = type;
	if (ft_strchr(filename, '\'') || ft_strchr(filename, '"'))
		new->was_quoted = 1;
	else
		new->was_quoted = 0;
	new->filename = ft_strdup(filename);
	remove_quotes_inplace(new->filename);
	new->next = NULL;
	return (new);
}

/*
* Appends a new redirection node to the end of a redirection list.
*
* Parameters:
* - list: A pointer to the head of the redirection list.
* - new: The redirection node to be added.
*/

void	add_redir(t_redir **list, t_redir *new)
{
	t_redir	*temp;

	if (!*list)
	{
		*list = new;
		return ;
	}
	temp = *list;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}
