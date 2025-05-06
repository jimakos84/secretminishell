/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:57:55 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/06 05:51:52 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int			count_args(t_list *tokens);
int			ft_is_in_single(const char *str, int n);
t_cmd		*allocate_cmd_node(void);
char		**allocate_args_array(int num_args);
char		*set_path_name(t_shell *mini, char *token);
char		*copy_var_value(t_shell *mini, char *token, int *index);
char		*expand_token(char *token, t_shell *mini);
static char	*string_build(char *s1, char *s2);
static char	*extract_value(t_shell *mini, char *token, int *index);
t_list		*handle_arg_or_redirection(t_cmd *cmd, t_list *current, int *i);
t_list		*fill_args_and_cmd(t_cmd *cmd, t_list *tokens, t_shell *mini);
void		parse_commands(t_shell *mini, t_list *tokens);
void		process_token_expansion(t_list *current, t_shell *mini);
void		handle_dollar(t_list *list, t_shell *mini);

/*
 * Parses and expands all tokens in the shell input.
 *
 * Parameters:
 * - mini: Pointer to shell context.
 *
 * Returns:
 * - Always returns 0 (success).
 */

int	parse_and_expand(t_shell *mini)
{
	handle_dollar(mini->tokens, mini);
	parse_commands(mini, mini->tokens);
	return (0);
}

/*
 * Traverses the token list and expands variables prefixed by '$'.
 *
 * Parameters:
 * - list: The list of input tokens.
 * - mini: Pointer to shell context.
 */

void	handle_dollar(t_list *list, t_shell *mini)
{
	t_list	*current;
	char	*str;

	current = list;
	str = NULL;
	while (current)
	{
		str = ft_strchr(current->token, '$');
		if (str && !ft_is_in_single(current->token, str - current->token))
			process_token_expansion(current, mini);
		current = current->next;
	}
}

/*
 * Expands a single token in the list if it is not within single quotes.
 *
 * Parameters:
 * - current: The current token node.
 * - mini: Pointer to shell context.
 */

void	process_token_expansion(t_list *current, t_shell *mini)
{
	char	*new_token;
	char	*old_token;

	if (current->in_single_quotes)
		return ;
	old_token = current->token;
	new_token = expand_token(old_token, mini);
	if (!new_token)
		return ;
	current->token = ft_strdup(new_token);
	free(old_token);
	free(new_token);
}

/*
 * Expands all variables within a given token string.
 *
 * Parameters:
 * - token: The input string containing potential variable references.
 * - mini: Pointer to shell context for environment access.
 *
 * Returns:
 * - A new string with all variables expanded.
 */

char	*expand_token(char *token, t_shell *mini)
{
	int		i;
	int		start;
	char	*result;
	char	*value;

	i = 0;
	start = 0;
	result = ft_strdup("");
	while (token && token[i])
	{
		if (token[i] == '$' && !ft_is_in_single(token, i))
		{
			result = string_build(result, ft_strnmdup(token, start, i));
			i++;
			value = copy_var_value(mini, token, &i);
			result = string_build(result, value);
			start = i;
		}
		else
			i++;
	}
	if (token[start])
		result = string_build(result, ft_strnmdup(token, start, i));
	return (result);
}

/*
 * Handles variable extraction during token expansion.
 *
 * Supports $? for last exit status and $$ for process ID.
 *
 * Parameters:
 * - mini: Pointer to shell context.
 * - token: The input string.
 * - index: Current parsing index, updated as characters are consumed.
 *
 * Returns:
 * - A newly allocated string with the variable's value or "$" if invalid.
 */

static char	*extract_value(t_shell *mini, char *token, int *index)
{
	char	*value;

	if (token && token[*index] == '?')
	{
		(*index)++;
		value = ft_itoa(mini->initenv->last_status);
		return (value);
	}
	if (token && token[*index] == '$')
	{
		(*index)++;
		value = ft_itoa(getpid());
		return (value);
	}
	if (!token[*index] || token[*index] == '"' || token[*index] == ' ')
	{
		value = ft_strdup("$");
		return (value);
	}
	return (NULL);
}

/*
 * Extracts the value of an environment variable or special variable from
 * a token.
 *
 * Handles special cases like $? and $$.
 *
 * Parameters:
 * - mini: Pointer to shell context.
 * - token: The string containing the variable.
 * - index: Pointer to the current index in the string, updated
 * as characters are parsed.
 *
 * Returns:
 * - A newly allocated string with the variable's value.
 */

char	*copy_var_value(t_shell *mini, char *token, int *i)
{
	int		start;
	char	*key;
	char	*value;

	start = *i;
	value = extract_value(mini, token, i);
	if (value)
		return (value);
	while (token[*i] && ((ft_isalnum(token[*i]) || token[*i] == '_')))
		(*i)++;
	key = ft_strnmdup(token, start, *i);
	value = extract_env_value(mini->initenv, key);
	if (!value)
		value = ft_strdup("");
	free(key);
	return (value);
}

/*
 * Checks if a given position in a string is inside single quotes.
 *
 * Parameters:
 * - str: The input string.
 * - n: The index to check.
 *
 * Returns:
 * - 1 if inside single quotes at position n, 0 otherwise.
 */

int	ft_is_in_single(const char *str, int n)
{
	int	in_single;
	int	i;

	in_single = 0;
	i = 0;
	while (str && str[i] && i != n)
	{
		if (str[i] == '\'' && !in_single)
			in_single = !in_single;
		else if (str[i] == '\'' && in_single)
			in_single = !in_single;
		i++;
	}
	return (in_single);
}

/*
 * Allocates and initializes a t_cmd structure with default values.
 *
 * Returns:
 * - A pointer to the allocated t_cmd structure, or NULL on failure.
 */

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

/*
 * Allocates memory for an array of argument strings.
 *
 * Parameters:
 * - num_args: The number of arguments to allocate space for.
 *
 * Returns:
 * - A pointer to the newly allocated array of strings, or NULL on failure.
 */

char	**allocate_args_array(int num_args)
{
	char	**args;

	args = malloc(sizeof(char *) * (num_args + 1));
	if (!args)
		return (NULL);
	return (args);
}

/*
 * Determines the redirection type based on a token string.
 *
 * Parameters:
 * - token: The redirection token string.
 *
 * Returns:
 * - An integer representing the redirection type, or 0
 * if not a redirection token.
 */

static int	set_type(char *token)
{
	int	type;

	if (ft_strncmp(token, ">", 1) == 0)
		type = OPRD_CMD;
	else if (ft_strncmp(token, ">>", 2) == 0)
		type = APRD_CMD;
	else if (ft_strncmp(token, "<", 1) == 0)
		type = IPRD_CMD;
	else if (ft_strncmp(token, "<<", 2) == 0)
		type = HDRD_CMD;
	else
		type = 0;
	return (type);
}

/*
 * Handles either an argument or a redirection token for a command.
 *
 * Redirection tokens are added to the redir_list; arguments are added to args.
 *
 * Parameters:
 * - cmd: Pointer to the current command structure.
 * - current: Current token node.
 * - i: Pointer to the argument index counter.
 *
 * Returns:
 * - The next token node to process.
 */

t_list	*handle_arg_or_redirection(t_cmd *cmd, t_list *current, int *i)
{
	int		type;
	t_redir	*redir;

	if (is_redirection_token(current->token))
	{
		if (!current->next)
			return (current);
		type = set_type(current->token);
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

/*
 * Fills command arguments and sets command path or builtin flags.
 *
 * Parameters:
 * - cmd: The command structure being filled.
 * - tokens: The list of tokens representing a command and its arguments.
 * - mini: Pointer to shell context.
 *
 * Returns:
 * - The token node following the end of the current command.
 */

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

/*
 * Counts the number of tokens in a linked list.
 *
 * Parameters:
 * - tokens: Pointer to the head of the token list.
 *
 * Returns:
 * - The number of tokens in the list.
 */

int	count_args(t_list *tokens)
{
	int		count;
	t_list	*current;

	current = tokens;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/*
 * Allocates and initializes a command structure based on a list of tokens.
 *
 * This function:
 * - Allocates a new t_cmd structure.
 * - Counts the number of tokens (arguments) for the command.
 * - Allocates an argument array of appropriate size.
 * - Frees and returns NULL if allocation fails or if there are no arguments.
 *
 * Parameters:
 * - token: Pointer to the token list representing a command and its arguments.
 *
 * Returns:
 * - A pointer to the initialized t_cmd structure, or NULL on failure.
 */

static t_cmd	*set_cmd(t_list *token)
{
	t_cmd	*cmd;
	int		num_args;

	cmd = allocate_cmd_node();
	if (!cmd)
		return (NULL);
	num_args = count_args(token);
	if (num_args == 0)
	{
		free(cmd);
		return (NULL);
	}
	cmd->args = allocate_args_array(num_args);
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

/*
 * Parses the list of tokens into command structures and links them.
 *
 * Parameters:
 * - mini: Pointer to shell context.
 * - tokens: The list of input tokens.
 */

void	parse_commands(t_shell *mini, t_list *tokens)
{
	t_list	*current;
	t_cmd	*cmd;

	current = tokens;
	while (current)
	{
		if (current->token == NULL || current->token[0] == '\0')
		{
			current = current->next;
			continue ;
		}
		cmd = set_cmd(current);
		if (!cmd)
			return ;
		current = fill_args_and_cmd(cmd, current, mini);
		mini->cmds = list_add_command(mini->cmds, cmd);
		mini->num_cmds++;
		if (current && contains_unquoted_char(current->token, '|'))
			current = current->next;
	}
}

/*
 * Resolves the executable path of a command token.
 *
 * Checks if the token is directly executable, otherwise searches in PATH.
 *
 * Parameters:
 * - mini: Pointer to the shell context.
 * - token: The command string.
 *
 * Returns:
 * - A string representing the full executable path,
 * or the token itself if already executable.
 */

char	*set_path_name(t_shell *mini, char *token)
{
	char	**path_dirs;
	char	*path;
	char	*temp;
	char	*full_path;
	int		i;

	i = 0;
	path = token;
	if (access(path, X_OK) == 0)
		return (path);
	path_dirs = ft_split(extract_env_value(mini->initenv, "PATH"), ':');
	while (path_dirs && path_dirs[i])
	{
		temp = ft_strjoin(path_dirs[i], "/");
		full_path = ft_strjoin(temp, path);
		free(temp);
		if (access(full_path, X_OK) == 0)
			break ;
		free(full_path);
		i++;
	}
	if (full_path)
		path = full_path;
	clear_array(path_dirs);
	return (path);
}

/*
 * Concatenates two strings and frees the first one.
 *
 * Parameters:
 * - s1: First string (will be freed).
 * - s2: Second string.
 *
 * Returns:
 * - A new string resulting from the concatenation.
 */

static char	*string_build(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}
