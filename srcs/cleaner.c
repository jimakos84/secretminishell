/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:48:20 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/25 14:08:24 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Function declaration of helper fuctions
*/
int		clear_and_exit(t_shell *mini);
int		clear_commands(t_cmd *cmds);
int		clear_tokens(t_list *tokens);
int		clear_array(char **array);

/*
* Frees all shell command structures, tokens, and the shell context.
*
* Parameters:
* - mini: Pointer to the shell context to be cleared.
*
* Returns:
* - Always returns 0.
*/

int	clear_and_exit(t_shell *mini)
{
	if (mini->cmds)
		clear_commands(mini->cmds);
	if (mini->tokens)
		clear_tokens(mini->tokens);
	mini->initenv = NULL;
	free(mini);
	return (0);
}

/*
* Frees a linked list of t_cmd structures, including command strings,
* arguments, and redirection lists.
*
* Parameters:
* - cmd: Pointer to the head of the command list.
*
* Returns:
* - Always returns 0.
*/

int	clear_commands(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->cmd)
			free(cmd->cmd);
		if (cmd->command)
			free(cmd->command);
		if (cmd->args)
		{
			clear_array(cmd->args);
			cmd->args = NULL;
		}
		if (cmd->redir_list)
		{
			free_redirections(cmd->redir_list);
			cmd->redir_list = NULL;
		}
		free(cmd);
		cmd = tmp;
	}
	return (0);
}

/*
* Frees a linked list of token nodes and their strings.
*
* Parameters:
* - tokens: Pointer to the head of the token list.
*
* Returns:
* - Always returns 0.
*/

int	clear_tokens(t_list *tokens)
{
	t_list	*current;
	t_list	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->token)
			free(current->token);
		free(current);
		current = next;
	}
	return (0);
}

/*
* Frees a NULL-terminated array of strings.
*
* Parameters:
* - array: The array to free.
*
* Returns:
* - Always returns 0.
*/

int	clear_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return (0);
	while (array[i])
	{
		if (array[i])
			free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	return (0);
}
