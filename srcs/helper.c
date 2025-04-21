#include "../includes/shell.h"

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
	mini->trim = NULL;
	mini->status = 0;
	if((status = input_validate(&input)))
		return (1);
	if((status = extract_tokens(&mini->tokens, input)))
		return (1);
	if((status = parse_and_expand(mini)))
		return (1);
	if((status = execute(mini)))
		return (1);
	if((status = clear_and_exit(mini)))
		return (1);
	return (status);
}
