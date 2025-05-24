/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:13:31 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/24 04:24:09 by tsomacha         ###   ########.fr       */
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
