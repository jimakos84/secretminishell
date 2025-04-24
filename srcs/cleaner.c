/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:48:20 by tsomacha          #+#    #+#             */
/*   Updated: 2025/04/23 12:53:31 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int	clear_commands(t_cmd *cmds);
int	clear_tokens(t_list *tokens);
int	clear_array(char **array);

int		clear_env(t_shell *mini)
{
	char	*name;
	char	*value;
	t_env	*current;
	t_env	*temp;

	current = mini->initenv->env;
	if (!current)
		return (1);
	while (current)
	{
		name = mini->initenv->env->name;
		value = mini->initenv->env->value;
		free(name);
		if (value)
			free(value);
		temp = current;
		current = current->next;
		free(temp);
	}
	return (0);
}

int	clear_and_exit(t_shell *mini)
{
	clear_commands(mini->cmds);
	clear_tokens(mini->tokens);
	//clear_env(mini);
	return (0);
}

int	clear_commands(t_cmd *cmds)
{
	t_cmd	*current;
	t_cmd	*cmd;

	current = cmds;
	while (current)
	{
		cmd = current;
		current = current->next;
		free(cmd->command);
	}
	return (0);
}

int	clear_tokens(t_list *tokens)
{
	t_list	*current;
	t_list	*token;

	current = tokens;
	while (current)
	{
		token = current;
		current = current->next;
		free(token);
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
		i++;
	}
	if (array)
		free(array);
	return (0);
}
