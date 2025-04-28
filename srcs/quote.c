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

char *remove_quotes(char *str) 
{
    int     i;
    int     len;
    int     j;
    char    *result;

    len = ft_strlen(str);
    j = 0;
    i = 0;
    if (str == NULL || len == 0)
        return (str);
    while ((str[i] == '"' || str[i] == '\'') && i < len)
        i++;
    while ((str[len - 1] == '"' || str[len - 1] == '\'') && len > i)
        len--;
    result = malloc(sizeof(char) * (len - i + 1));
    if (!result) 
        return (NULL);
    while (i < len) 
        result[j++] = str[i++];
    result[j] = '\0';
    return (result);
}


