/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:57:55 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/04 05:15:41 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

void	expand(t_shell *mini, t_list *list);
int		get_num_args(char *token);
void	get_args(char **args, char *token, int size);
char	*get_command(char *token);
char	*set_path_name(t_shell *mini, char *token);
char	**set_arg_array(int num_args, char *token, char *cmd);
void	parse_commands(t_shell *mini, t_list *tokens);

int	parse_and_expand(t_shell *mini)
{
	handle_dollar(mini->tokens, mini);

	parse_commands(mini, mini->tokens);
	return (0);
}

t_cmd	*allocate_cmd_node(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->cmd = NULL;
	cmd->command = NULL;
	cmd->num_args = 0;
	cmd->is_builtin = 0;
	cmd->type = SMPL_CMD;
	cmd->redir_list = NULL;
	cmd->next = NULL;
	return (cmd);
}

char	**allocate_args_array(int num_args)
{
	char	**args;

	args = malloc(sizeof(char *) * (num_args + 1));
	if (!args)
		return (NULL);
	return (args);
}

t_list	*handle_arg_or_redirection(t_cmd *cmd, t_list *current, int *i)
{
	int type;
	t_redir *redir;

	if (is_redirection_token(current->token))
	{
		if (!current->next)
			return (current);

		if (ft_strncmp(current->token, ">", ft_strlen(current->token)) == 0)
			type = OPRD_CMD;
		else if (ft_strncmp(current->token, ">>", ft_strlen(current->token)) == 0)
			type = APRD_CMD;
		else if (ft_strncmp(current->token, "<", ft_strlen(current->token)) == 0)
			type = IPRD_CMD;
		else if (ft_strncmp(current->token, "<<", ft_strlen(current->token)) == 0)
			type = HDRD_CMD;
		else
			type = 0;
		redir = create_redir_node(type, current->next->token);
		add_redir(&cmd->redir_list, redir);
		current = current->next;
	}
	else
	{
		cmd->args[*i] = ft_strdup(current->token);
		remove_quotes_inplace(cmd->args[*i]);
		(*i)++;
	}
	return (current->next);
}


t_list	*fill_args_and_cmd(t_cmd *cmd, t_list *tokens, t_shell *mini)
{
	int		i;
	t_list	*current;

	i = 0;
	current = tokens;
	cmd->cmd = ft_strdup(current->token);
	remove_quotes_inplace(cmd->cmd);
	if (builtin_cmd(cmd->cmd))
		cmd->is_builtin = 1;
	else
		cmd->command = set_path_name(mini, cmd->cmd);
	cmd->args[i++] = ft_strdup(cmd->cmd);
	current = current->next;
	while (current && !contains_unquoted_char(current->token, '|'))
		current = handle_arg_or_redirection(cmd, current, &i);
	cmd->args[i] = NULL;
	cmd->num_args = i;
	return (current);
}

int	count_args(t_list *tokens)
{
	int		count;
	t_list	*tmp;

	tmp = tokens;
	count = 0;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

void	parse_commands(t_shell *mini, t_list *tokens)
{
	t_list	*current;
	t_cmd	*cmd;
	int		num_args;

	current = tokens;
	while (current)
	{
		if (current->token == NULL || current->token[0] == '\0')
		{
			current = current->next;
			continue ;
		}
		cmd = allocate_cmd_node();
		if (!cmd)
			return ;
		num_args = count_args(current);
		if (num_args == 0)
		{
			free(cmd);
			return ;
		}
		cmd->args = allocate_args_array(num_args);
		if (!cmd->args)
		{
			free(cmd);
			return ;
		}
		current = fill_args_and_cmd(cmd, current, mini);
		mini->cmds = list_add_command(mini->cmds, cmd);
		mini->num_cmds++;
		if (current && contains_unquoted_char(current->token, '|'))
			current = current->next;
	}
}

char	*set_path_name(t_shell *mini, char *token)
{
	char	**path_dirs;
	char	*command;
	char	*path;
	char	*path_value;
	char	*temp;
	char	*full_path;
	int		i;

	i = 0;
	command = token;
	path = command;
	if (access(path, X_OK) == 0)
		return (path);
	else
		path = NULL;
	path_value = extract_env_value(mini->initenv, "PATH");
	if (!path_value)
	{
		return (command);
	}
	path_dirs = ft_split(path_value, ':');
	if (!path_dirs)
		return (command);
	while (path_dirs[i])
	{
		temp = ft_strjoin(path_dirs[i], "/");
		full_path = ft_strjoin(temp, command);
		free(temp);
		if (access(full_path, X_OK) == 0)
		{
			clear_array(path_dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	clear_array(path_dirs);
	return (command);
}

static int	expand_exit_status(int *j, char *expanded, int status)
{
	char	*status_str;
	int		len;

	status_str = ft_itoa(status);
	if (!status_str)
		return (0);
	len = ft_strlcpy(&expanded[*j], status_str, 4096 - *j);
	*j += len;
	free(status_str);
	return (1);
}


char	*copy_var_value(char *token, int *i, int *j, char *expanded, t_shell *mini)
{
	int		k;
	char	var_name[1024];
	char	*var_value;

	if (token[*i] == '?')
	{
		(*i)++;
		expand_exit_status(j, expanded, mini->initenv->last_status);
		return (expanded);
	}
	if (!token[*i] || !(ft_isalnum(token[*i]) || token[*i] == '_'))
	{
		expanded[(*j)++] = '$';  // Just copy the literal $
		return (expanded);
	}
	k = 0;
	while (token[*i] && (ft_isalnum(token[*i]) || token[*i] == '_'))
	{
		var_name[k] = token[*i];
		k++;
		(*i)++;
	}
	var_name[k] = '\0';
	var_value = extract_env_value(mini->initenv, var_name);
	if (var_value)
		*j += ft_strlcpy(&expanded[*j], var_value, 4096 - *j);
	return (expanded);
}

char	*expand_token(char *token, t_shell *mini)
{
	char	*expanded;
	int		i;
	int		j;

	i = 0;
	j = 0;
	expanded = malloc(4096);
	if (!expanded)
		return (NULL);
	while (token[i])
	{
		if (token[i] == '$')
		{
			i++;
			expanded = copy_var_value(token, &i, &j, expanded, mini);
		}
		else
		{
			expanded[j] = token[i];
			j++;
			i++;
		}
	}
	expanded[j] = '\0';
	return (expanded);
}

void	process_token_expansion(t_list *curr, t_shell *mini)
{
	char	*new_token;
	char	*old_token;

	if (curr->in_single_quotes)
		return ;
	old_token = curr->token;
	new_token = expand_token(old_token, mini);
	if (!new_token)
		return ;
	curr->token = ft_strdup(new_token);
	free(old_token);
	free(new_token);
}

void	handle_dollar(t_list *list, t_shell *mini)
{
	t_list	*curr;

	curr = list;
	while (curr)
	{
		process_token_expansion(curr, mini);
		curr = curr->next;
	}
}



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
