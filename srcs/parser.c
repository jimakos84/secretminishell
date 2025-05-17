/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:13:31 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/17 01:55:50 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Function declaration of helper fuctions
*/
char	*string_build(char *s1, char *s2);
char	*extract_value(t_shell *mini, char *token, int *index);
t_list	*handle_arg_or_redirection(t_cmd *cmd, t_list *current, int *i);
t_list	*fill_args_and_cmd(t_cmd *cmd, t_list *tokens, t_shell *mini);

/*
* Parses and expands all tokens in the shell input.
*
* Parameters:
* - mini: Pointer to shell context.
*
* Returns:
* - Always returns 0 (success).
*/

int	parse_and_expand(t_shell *mini)
{
	expansion_preprocess(mini->tokens);
	handle_dollar(mini->tokens, mini);
	parse_commands(mini, mini->tokens);
	return (0);
}

/*
* Handles variable extraction during token expansion.
*
* Supports $? for last exit status and $$ for process ID.
*
* Parameters:
* - mini: Pointer to shell context.
* - token: The input string.
* - index: Current parsing index, updated as characters are consumed.
*
* Returns:
* - A newly allocated string with the variable's value or "$" if invalid.
*/

char	*extract_value(t_shell *mini, char *token, int *index)
{
	char	*value;

	if (token && token[*index] == '?')
	{
		(*index)++;
		value = ft_itoa(mini->initenv->last_status);
		return (value);
	}
	if (token && token[*index] == '$')
	{
		(*index)++;
		value = ft_itoa(ft_getpid());
		return (value);
	}
	if (!token[*index] || token[*index] == '"' || token[*index] == ' ')
	{
		value = ft_strdup("$");
		return (value);
	}
	return (NULL);
}

/*
 * Handles either an argument or a redirection token for a command.
 *
 * Redirection tokens are added to the redir_list; arguments are added to args.
 *
 * Parameters:
 * - cmd: Pointer to the current command structure.
 * - current: Current token node.
 * - i: Pointer to the argument index counter.
 *
 * Returns:
 * - The next token node to process.
 */

t_list	*handle_arg_or_redirection(t_cmd *cmd, t_list *current, int *i)
{
	int		type;
	t_redir	*redir;

	if (is_redirection_token(current->token))
	{
		if (!current->next)
			return (current);
		type = set_type(current->token);
		if (current->next->token[0])
			redir = create_redir_node(type, current->next->token);
		else
			return (NULL);
		add_redir(&cmd->redir_list, redir);
		current = current->next;
	}
	else
	{
		cmd->args[*i] = ft_strdup(current->token);
		remove_quotes_inplace(cmd->args[*i]);
		(*i)++;
	}
	return (current->next);
}

/*
 * Fills command arguments and sets command path or builtin flags.
 *
 * Parameters:
 * - cmd: The command structure being filled.
 * - tokens: The list of tokens representing a command and its arguments.
 * - mini: Pointer to shell context.
 *
 * Returns:
 * - The token node following the end of the current command.
 */

t_list	*fill_remaining_args(t_cmd *cmd, t_list *tokens, int *i)
{
	t_list	*current;

	current = tokens;
	while (current && !contains_unquoted_char(current->token, '|'))
		current = handle_arg_or_redirection(cmd, current, i);
	cmd->args[*i] = NULL;
	cmd->num_args = *i;
	return (current);
}

t_list	*fill_args_and_cmd(t_cmd *cmd, t_list *tokens, t_shell *mini)
{
	int		i;
	t_list	*current;

	i = 0;
	current = process_redirections(cmd, tokens, &i);
	if (!current)
		return (NULL);
	if (!is_redirection_token(current->token))
	{
		init_cmd_from_token(cmd, current, mini, &i);
		current = current->next;
	}
	current = fill_remaining_args(cmd, current, &i);
	return (current);
}
