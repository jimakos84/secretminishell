#include "includes/shell.h"

int print_cmd(t_cmd *cmd);
int print_token(t_list *token);
int print_all(t_shell *mini);

int print_token(t_list *token)
{
	t_list *current = token;

	while(current)
	{
		printf("token : {%s}\n", current->token);
		current = current->next;
	}
	if(!current)
		printf("token : {(null)}}\n");
	return (0);
}

int print_cmd(t_cmd *cmd)
{
	t_cmd *current = cmd;

	int j = 0;
	if(current)
	{
		printf("cmd : %s\n", current->cmd);
		printf("command : %s\n", current->command);
		printf("is_builtin : %d\n", current->is_builtin);
		printf("num_args : %d\n", current->num_args);
		printf("type : %d\n", current->type);
		if(current->args)
		{
			printf("args list\n");
			while(current->args[j])
			{
				printf("args[%d] : %s\n", j, current->args[j]);
				j++;
			}
		}
		if(current->redir_list)
		{
			printf("redir_list\n");
			t_redir *temp = current->redir_list;
			while(temp)
			{
				printf("file name : %s\n", temp->filename);
				printf("type : %d\n", temp->type);
				printf("was quoted : %d\n", temp->was_quoted);
				temp = temp->next;
			}
		}
	}
	else
		printf("cmd is (null)\n");
	printf("------------------------------------------------ eof ---\n");
	return (0);
}
