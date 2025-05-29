/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 04:38:15 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/29 09:23:28 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
 * Function declaration of helper fuctions
*/
void	init_rd_cmd(t_cmd *cmd);
void	parse_commands(t_shell *mini, t_list *tokens);
t_list	*fill_args_and_cmd(t_cmd *cmd, t_list *tokens, t_shell *mini);
t_list	*fill_remaining_args(t_cmd *cmd, t_list *tokens, int *i);

int	check_for_pipe_token(t_list *node)
{
	if (node)
	{
		if (node->token)
		{
			if (ft_strcmp(node->token, "|") == 0)
				return (1);
		}
		return (0);
	}
	return (0);
}

t_list	*fill_args_and_cmd(t_cmd *cmd, t_list *tokens, t_shell *mini)
{
	int		i;
	t_list	*current;

	i = 0;
	current = tokens;
	if (check_for_pipe_token(current))
		return (current);
	if (is_redirection_token(current->token))
	{
		if (!cmd || !cmd->cmd)
			init_rd_cmd(cmd);
		current = handle_arg_or_redirection(cmd, current, &i);
	}
	current = init_cmd_from_token(cmd, current, mini, &i);
	current = fill_remaining_args(cmd, current, &i);
	return (current);
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
		current = current->next;
		redir = create_redir_node(type, current->token);
		add_redir(&cmd->redir_list, redir);
		current = current->next;
	}
	else
	{
		if (current && current->token)
		{
			cmd->args[*i] = ft_strdup(current->token);
			remove_quotes_inplace(cmd->args[*i]);
			(*i)++;
		}
		current = current->next;
	}
	return (current);
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
	if (!current)
		return (current);
	if (is_redirection_token(current->token))
		return (current);
	while (current && !check_for_pipe_token(current))
		current = handle_arg_or_redirection(cmd, current, i);
	cmd->args[*i] = NULL;
	cmd->num_args = *i;
	return (current);
}

void	init_rd_cmd(t_cmd *cmd)
{
	cmd->cmd = ft_strdup("rd");
	if (!cmd->cmd)
		return ;
}
