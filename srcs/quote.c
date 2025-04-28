#include "../includes/shell.h"

//static char **set_arg_array_quoted(t_shell *mini, char *token);
//static char *set_command(t_shell *mini, char *token);

// t_cmd *handle_quoted(t_shell *mini, char *token)
// {
// 	t_cmd *cmd = malloc(sizeof(t_cmd));
// 	if(!cmd)
// 		return (NULL);
// 	cmd->type = SMPL_CMD;
// 	cmd->command = set_path_name(mini, token);
// 	cmd->num_args = 2;
// 	cmd->filename = NULL;
// 	cmd->args = set_arg_array_quoted(mini, token);
// 	cmd->next = NULL;
// 	return (cmd);
// }

// static char **set_arg_array_quoted(t_shell *mini, char *token)
// {
// 	char **args = malloc(sizeof(char *) * 2);
// 	args[0] = set_command(mini, token);
// 	args[1] = NULL;
// 	return (args);
// }

// static char *set_command(t_shell *mini, char *token)
// {
// 	int i = 0;
// 	char *cmd;

// 	while(token && token[i] && !ft_isspace(token[i]))
// 		i++;
// 	if(token[i])
// 		cmd = set_path_name(mini, token);
// 	else
// 		cmd = set_path_name(mini, token);
// 	return (cmd);
// }

bool contains_unquoted_char(char *str, char symbol)
{
	bool in_single = false;
	bool in_double = false;

	for (int i = 0; str[i]; i++)
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		else if (str[i] == symbol && !in_single && !in_double)
			return true;
	}
	return false;
}



int check_if_quoted(char *input)
{
	int len = ft_strlen(input);

	if(len == 0)
		return (0);
	else if(input[0] == '\'' && input[len-1] == '\'')
		return (1);
	else if(input[0] == '"' && input[len-1] == '"')
		return (1);
	else
		return (0);
}

void remove_quotes_inplace(char *str)
{
    int i = 0;
    int j = 0;
    int quote;

    if (!str)
        return;
    quote = 0;
    while (str[i])
    {
        if ((str[i] == '\'' || str[i] == '"') && !quote)
            quote = str[i++];
        else if (str[i] == quote)
        {
            quote = 0;
            i++;
        }
        else
            str[j++] = str[i++];
    }
    str[j] = '\0';
}



