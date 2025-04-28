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

void clear_env(t_env *env)
{
    t_env *tmp;

    while (env)
    {
        tmp = env->next;
        free(env->name);
        free(env->value);
        free(env);
        env = tmp;
    }
}


int clear_and_exit(t_shell *mini)
{
    // if (mini->cmds)
    // {
    //     clear_commands(mini->cmds);
    //     mini->cmds = NULL;
    // }
    if (mini->tokens)
    {
        clear_tokens(mini->tokens);
        mini->tokens = NULL;
    }
    free(mini);
    return (0);
}



int clear_commands(t_cmd *cmd)
{
    t_cmd *tmp;
    int i;

    while (cmd)
    {
        tmp = cmd->next;
        if (cmd->cmd)
            free(cmd->cmd);
        if (cmd->command)
            free(cmd->command);
        if (cmd->filename)
            free(cmd->filename);
        if (cmd->args)
        {
            i = 0;
            while (cmd->args[i])
            {
                free(cmd->args[i]);
                i++;
            }
            free(cmd->args);
        }
        free(cmd);
        cmd = tmp;
    }
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
			free(current->token);  // FREE THE STRING INSIDE
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
		i++;
	}
	if (array)
		free(array);
	return (0);
}
