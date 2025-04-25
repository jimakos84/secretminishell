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

char *enclosed_in_quotes(char *input)
{
	char *str = NULL;
	int len = ft_strlen(input);

	str = ft_strnstr(input, "'", len);
	if(str)
		return (str);
	str = ft_strnstr(input, "\"", len);
	if(str)
		return (str);
	return (str);
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

char *remove_quotes(char *str) {
    int i = 0, j = 0, len = strlen(str);
    char *result;

    // Handle case where input might be empty
    if (str == NULL || len == 0) {
        return str;
    }

    // Find where the quotes start and end (skip leading quotes)
    while ((str[i] == '"' || str[i] == '\'') && i < len) {
        i++;
    }

    // Find where the quotes end (skip trailing quotes)
    while ((str[len - 1] == '"' || str[len - 1] == '\'') && len > i) {
        len--;
    }

    // Allocate memory for the cleaned string (null-terminated)
    result = malloc(sizeof(char) * (len - i + 1)); // +1 for the null-terminator
    if (!result) {
        return NULL;  // Memory allocation failed
    }

    // Copy the cleaned string into the result
    for (j = 0; i < len; i++, j++) {
        result[j] = str[i];
    }

    // Null-terminate the result string
    result[j] = '\0';

    return result;
}


