#include "../includes/shell.h"

int clear_commands(t_cmd *cmds);
int clear_tokens(t_list *tokens);
int clear_array(char **array);

int clear_and_exit(t_shell *mini)
{
	clear_commands(mini->cmds);
	clear_tokens(mini->tokens);
	if (mini->initenv->copy_env)
		clear_array(mini->initenv->copy_env);
	return (0);
}

int clear_commands(t_cmd *cmds)
{
	t_cmd *current = cmds, *cmd;
	while(current)
	{
		cmd = current;
		current = current->next;
		free(cmd->command);
	}
	return (0);
}
int clear_tokens(t_list *tokens)
{
	t_list *current = tokens, *token;
	while(current)
	{
		token = current;
		current = current->next;
		free(token);
	}
	return (0);
}

int clear_array(char **array)
{
	int i = 0;
	while(array && array[i])
	{
		if(array[i])
			free(array[i]);
		i++;
	}
	if(array)
		free(array);
	return (0);
}
