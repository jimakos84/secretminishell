#include "../includes/shell.h"

void expand(t_shell *mini, t_list *list);
int get_num_args(char *token);
void get_args(char **args, char *token, int size);
char *get_command(char *token);
char *set_path_name(t_shell *mini, char *token);
char **set_arg_array(int num_args, char *token, char *cmd);
void parse_commands(t_shell *mini, t_list *tokens);

int parse_and_expand(t_shell *mini)
{
    parse_commands(mini, mini->tokens);
    return (0);
}

t_cmd *allocate_cmd_node(void)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return NULL;
    cmd->args = NULL;
    cmd->cmd = NULL;
    cmd->command = NULL;
    cmd->num_args = 0;
	cmd->is_builtin = 0;
    cmd->type = SMPL_CMD;
    cmd->filename = NULL;
    cmd->next = NULL;
    return cmd;
}

char **allocate_args_array(int num_args)
{
    char **args = malloc(sizeof(char *) * (num_args + 1));
    if (!args)
        return NULL;
    return args;
}

t_list *fill_args_and_cmd(t_cmd *cmd, t_list *tokens, t_shell *mini)
{
    int i = 0;
    t_list *current = tokens;

    cmd->cmd = remove_quotes(ft_strdup(current->token));
    if (builtin_cmd(cmd->cmd))
        cmd->is_builtin = 1;
    else
	{
        cmd->command = set_path_name(mini, cmd->cmd);
	}
	cmd->args[i++] = ft_strdup(cmd->cmd);
    current = current->next;
    while (current && !contains_unquoted_char(current->token, '|'))
    {
        if (is_redirection_token(current->token))
        {
            if (current->next)
            {
                cmd->filename = remove_quotes(ft_strdup(current->next->token));
                if (ft_strncmp(current->token, ">", ft_strlen(current->token)) == 0)
                    cmd->type = OPRD_CMD;
                else if (ft_strncmp(current->token, "<", ft_strlen(current->token)) == 0)
                    cmd->type = IPRD_CMD;
            }
            current = current->next; // skip filename
        }
        else
        {
            cmd->args[i] = remove_quotes(ft_strdup(current->token));
            i++;
        }
        current = current->next;
    }

    cmd->args[i] = NULL;
    cmd->num_args = i;
    
    return current; // return where parsing stopped
}


int count_args(t_list *tokens)
{
    int count = 0;
    t_list *tmp = tokens;
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }
    return count;
}

void parse_commands(t_shell *mini, t_list *tokens)
{
    t_list *current = tokens;

    while (current)
    {
        t_cmd *cmd = allocate_cmd_node();
        if (!cmd)
            return;

        int num_args = count_args(current);
        if (num_args == 0)
        {
            free(cmd);
            return;
        }

        cmd->args = allocate_args_array(num_args);
        if (!cmd->args)
        {
            free(cmd);
            return;
        }

        // 🔥 current is now updated after parsing!
        current = fill_args_and_cmd(cmd, current, mini);

        mini->cmds = list_add_command(mini->cmds, cmd);
        mini->num_cmds++;

        // If current is on a pipe token ("|"), skip it
        if (current && contains_unquoted_char(current->token, '|'))
            current = current->next;
    }
}

char *set_path_name(t_shell *mini, char *token)
{
	char **path_dirs;
	char *command;
	char *path;
	int i;

	i = 0;
	command = token;
	path = command;
	if (access(path, X_OK) == 0)
		return (path);
	else
		path = NULL;
	char *path_value = extract_env_value(mini->initenv, "PATH");
	if (!path_value)
	{
		return (command); // Just return the raw token
	}
	path_dirs = ft_split(path_value, ':');
	if (!path_dirs)
		return (command);

	while (path_dirs[i])
	{
		char *temp = ft_strjoin(path_dirs[i], "/");
		char *full_path = ft_strjoin(temp, command);
		free(temp);
		if (access(full_path, X_OK) == 0)
		{
			// Found it!
			clear_array(path_dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	clear_array(path_dirs);
	return (command); // Not found, return original
}

// void handle_dollar(t_list *list, t_shell *mini)
// {
//     t_list *current;
//     char 	*token;
//     char 	*expanded_token;
//     int 	i;
// 	int		j;
// 	int 	var_start;
// 	int 	var_len;
// 	char 	*var_value;
// 	int		k;

// 	current = list;
// 	token = current->token;
// 	i = 0;
// 	j = 0;
// 	expanded_token = malloc(4128+ 1);
// 	if (!expanded_token)
//         return ;
//     while (token[i]) {
//         if (token[i] == '$') {
//             i++;
// 	 	var_start = i;
//         while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
//             i++;
//         var_len = i - var_start;
//         char var_name[var_len + 1];
//         ft_strlcpy(var_name, token + var_start, var_len + 1);
// 		var_value = extract_env_value(mini->initenv, var_name);
// 		k = 0;
//         while (var_value[k])
//             expanded_token[j++] = var_value[k++];
//         } else
//             expanded_token[j++] = token[i++];
//     }
//     expanded_token[j] = '\0';
// 	free(current->token);
// 	expanded_token = ft_strtrim(expanded_token, " ");
// 	current->token = expanded_token;
// }

// void expand(t_shell *mini, t_list *list)
// {
// 	t_list *current = list;
// 	t_cmd *cmd = NULL;

// 	while (current)
// {
// 	handle_dollar(current, mini);

// 	printf("token before output handling: [%s]\n", current->token);
// 	if (contains_unquoted_char(current->token, '>'))
// 		cmd = handel_output(mini, current->token);
// 	else if (contains_unquoted_char(current->token, '<'))
// 		cmd = handel_input(mini, current->token);
// 	else
// 		cmd = handel_pipe(mini, current);

// 	mini->cmds = list_add_command(mini->cmds, cmd);
// 	mini->num_cmds++;
// 	cmd = NULL;
// 	current = current->next;
// }
// }

// t_cmd *handel_pipe(t_shell *mini, t_list *current)
// {
// 	t_cmd *cmd = malloc(sizeof(t_cmd));
// 	if(!cmd)
// 			return (NULL);
// 	cmd->type = set_command_type(current->token);
// 	cmd->cmd = get_command(current->token);
// 	cmd->command = set_path_name(mini, cmd->cmd);
// 	cmd->num_args = get_num_args(current->token);
// 	cmd->args = set_arg_array(cmd->num_args, current->token, cmd->cmd);
// 	cmd->filename = NULL;
// 	cmd->next = NULL;
// 	return (cmd);
// }

// char **set_arg_array(int num_args, char *token, char *cmdpath)
// {
// 	char **args = malloc(sizeof(char*) * (num_args + 1));
// 	if(!args)
// 		return (NULL);
// 	args[0] = cmdpath; // Use cmdpath ("/usr/bin/echo") as the first argument
// 	if(num_args > 1)
// 		get_args(args, token, num_args);  // Get the arguments (e.g., "hello world")
// 	args[num_args] = NULL;
// 	return (args);
// }

// char *get_command(char *token)
// {
//     int i = 0;
//     int start = 0;
//     char *command;
//     int len = strlen(token);
//     int in_quote = quotes_checker(token, len);  // Check for any open quotes

//     // Skip any leading spaces
//     while (token[i] == ' ') {
//         i++;
//     }
//     start = i;

//     // Traverse the string to find the first space outside of quotes
//     while (token[i])
//     {
//         if (token[i] == '\'' || token[i] == '"')
//         {
//             // If we're inside quotes, continue without splitting
//             if (!in_quote)
//                 in_quote = token[i];  // Start of a quote
//             else if (in_quote == token[i])
//                 in_quote = 0;  // End of the quote
//         }
//         else if (token[i] == ' ' && in_quote == 0)
//         {
//             break;  // Found a space outside of quotes, we can split here
//         }
//         i++;
//     }

//     // Allocate memory for the command (including null terminator)
//     command = malloc(sizeof(char) * (i - start + 1));
//     if (!command) {
//         return NULL;  // Memory allocation failure
//     }

//     // Copy the part before the first space or end of the string
//     for (int j = 0; j < (i - start); j++) {
//         command[j] = token[start + j];
//     }
// 	command = remove_quotes(command);
//     // Null-terminate the string
//     command[i - start] = '\0';

//     return command;
// }

// void get_args(char **args, char *token, int size)
// {
// 	int i = 0, k = 0, start = 0, step = 1;
// 	int in_quote = 0;
// 	char quote_char = '\0';

// 	// Skip the command name (first word)
// 	while (token[i] && token[i] != ' ')
// 		i++;

// 	while (step < size && token[i])
// 	{
// 		// Skip leading spaces
// 		while (token[i] && token[i] == ' ')
// 			i++;

// 		start = i;
// 		in_quote = 0;
// 		quote_char = '\0';

// 		// Find the end of this argument
// 		while (token[i])
// 		{
// 			if (!in_quote && (token[i] == '\'' || token[i] == '"'))
// 			{
// 				in_quote = 1;
// 				quote_char = token[i];
// 			}
// 			else if (in_quote && token[i] == quote_char)
// 			{
// 				in_quote = 0;
// 			}
// 			else if (!in_quote && token[i] == ' ')
// 			{
// 				break;
// 			}
// 			i++;
// 		}

// 		// Allocate and copy argument
// 		args[step] = malloc(sizeof(char) * (i - start + 1));
// 		if (!args[step])
// 			return;

// 		k = 0;
// 		while ((start + k) < i)
// 		{
// 			args[step][k] = token[start + k];
// 			k++;
// 		}
// 		args[step][k] = '\0';
// 		args[step] = remove_quotes(args[step]); // remove " or '

// 		step++;
// 	}
// }




// int get_num_args(char *token)
// {
// 	int i = 0, count = 1, flag = 0;
// 	while(token && token[i])
// 	{
// 		if(token[i] == ' ' && !flag)
// 		{
// 			count++;
// 			flag = 1;
// 		}
// 		else if(token[i] != ' ' && flag)
// 			flag = 0;
// 		i++;
// 	}
// 	return (count);
// }

