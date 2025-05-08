/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:48:20 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/08 11:39:09 by dvlachos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int		clear_commands(t_cmd *cmds);
int		clear_tokens(t_list *tokens);
int		clear_array(char **array);
void	free_redirections(t_redir *redir_list);

void	clear_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{	
		tmp = env->next;
		free(env->name);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	free_redirections(t_redir *redir_list)
{
	t_redir	*tmp;

	while (redir_list)
	{
		tmp = redir_list->next;
		if (redir_list->filename)
			free(redir_list->filename);
		free(redir_list);
		redir_list = tmp;
	}
}

int	clear_and_exit(t_shell *mini)
{
	clear_commands(mini->cmds);
	clear_tokens(mini->tokens);
	free(mini);
	return (0);
}

int	clear_commands(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	i = 0;
	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->cmd)
			free(cmd->cmd);
		if (cmd->args)
		{
			clear_array(cmd->args);
		}
		if (cmd->redir_list)
		{
			free_redirections(cmd->redir_list);
			cmd->redir_list = NULL;
		}
		free(cmd);
		cmd = tmp;
	}
	cmd = NULL;
	return (0);
}

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

int	clear_array(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
	{
		if (array[i])
			free(array[i]);
		array[++i] = NULL;
	}
	if (array)
		free(array);
	return (0);
}
