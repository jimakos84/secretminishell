/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:13:55 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/29 09:21:16 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
 * Function declaration of helper fuctions
*/
int		set_type(char *token);
t_cmd	*set_cmd(t_list *token);

/*
* Determines the redirection type based on a token string.
*
* Parameters:
* - token: The redirection token string.
*
* Returns:
* - An integer representing the redirection type, or 0
* if not a redirection token.
*/

int	set_type(char *token)
{
	int	type;

	if (ft_strncmp(token, ">", 2) == 0)
		type = OPRD_CMD;
	else if (ft_strncmp(token, ">>", 3) == 0)
		type = APRD_CMD;
	else if (ft_strncmp(token, "<", 2) == 0)
		type = IPRD_CMD;
	else if (ft_strncmp(token, "<<", 3) == 0)
		type = HDRD_CMD;
	else
		type = 0;
	return (type);
}

/*
* Allocates and initializes a command structure based on a list of tokens.
*
* This function:
* - Allocates a new t_cmd structure.
* - Counts the number of tokens (arguments) for the command.
* - Allocates an argument array of appropriate size.
* - Frees and returns NULL if allocation fails or if there are no arguments.
*
* Parameters:
* - token: Pointer to the token list representing a command and its arguments.
*
* Returns:
* - A pointer to the initialized t_cmd structure, or NULL on failure.
*/

t_cmd	*set_cmd(t_list *token)
{
	t_cmd	*cmd;
	int		num_args;

	cmd = allocate_cmd_node();
	if (!cmd)
		return (NULL);
	num_args = count_args(token);
	if (num_args == 0)
	{
		free(cmd);
		return (NULL);
	}
	cmd->args = allocate_args_array(num_args);
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

char	*find_cmd(t_shell *mini, t_list *tokens)
{
	char	*cmd;

	while (tokens)
	{
		cmd = set_path_name(mini, tokens->token);
		if (ft_strchr(cmd, '/'))
			return (ft_strrchr(cmd, '/') + 1);
		tokens = tokens->next;
	}
	if (!cmd[0])
		return (NULL);
	return (cmd);
}

t_list	*init_cmd_from_token(t_cmd *cmd, t_list *tokens, t_shell *mini, int *i)
{
	t_list	*current;

	current = tokens;
	if (current && check_for_pipe_token(current))
		return (current);
	if (!current)
		return (current);
	if (is_redirection_token(current->token))
		return (current);
	remove_quotes_inplace(current->token);
	cmd->cmd = ft_strdup(current->token);
	if (!cmd->cmd)
		return (current);
	if (builtin_cmd(cmd->cmd))
		cmd->is_builtin = 1;
	else
		cmd->command = set_path_name(mini, cmd->cmd);
	cmd->args[(*i)++] = ft_strdup(cmd->cmd);
	current = current->next;
	return (current);
}
