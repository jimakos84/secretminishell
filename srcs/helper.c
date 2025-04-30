#include "../includes/shell.h"

void		printcmdtokens(t_shell *mini)
{
	if (!mini)
	{	
		printf("NULL MINI\n");
		return ;
	}
	int	i;

	i = 0;
	while (mini->cmds && mini->cmds->args[i])
	{
		printf("command args: %s\n", mini->cmds->args[i++]);
	}
	while (mini->tokens)
	{
		printf("tokens: %s\n", mini->tokens->token);
		mini->tokens = mini->tokens->next;
	}
	// while (mini->cmds->redir_list)
	// {
	// 	printf("redirection:%s\n", mini->cmds->redir_list->filename);
	// 	mini->cmds->redir_list = mini->cmds->redir_list->next;
	// }
}

int activate_shell(char *input, t_initenv *env)
{
	int status = 0;
	t_shell *mini = malloc(sizeof(t_shell));
	if(!mini)
		return (1);
	mini->num_cmds = 0;
	mini->tokens = NULL;
	mini->cmds = NULL;
	mini->initenv = env;
	mini->status = 0;
	mini->cmds = NULL;
	if((status = input_validate(&input)))
	 	return (status);
	if((status = extract_tokens(&mini->tokens, input)))
		return (status);
	if((status = parse_and_expand(mini)))
		return (status);
	if((status = execute(mini)))
		return (status);
	//printcmdtokens(mini);
	if((status = clear_and_exit(mini)))
		return (status);
	return (status);
}

