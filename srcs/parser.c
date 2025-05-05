/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:57:55 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/05 05:54:47 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int	ft_is_in_single(const char *str, int n);
t_cmd	*allocate_cmd_node(void);
char	**allocate_args_array(int num_args);
t_list	*handle_arg_or_redirection(t_cmd *cmd, t_list *current, int *i);
t_list	*fill_args_and_cmd(t_cmd *cmd, t_list *tokens, t_shell *mini);
int	count_args(t_list *tokens);
char	*set_path_name(t_shell *mini, char *token);
void	parse_commands(t_shell *mini, t_list *tokens);
char	*copy_var_value(t_shell *mini, char *token, int *index);
char	*expand_token(char *token, t_shell *mini);
void	process_token_expansion(t_list *current, t_shell *mini);
void	handle_dollar(t_list *list, t_shell *mini);
static char	*string_build(char *s1, char *s2);

int	parse_and_expand(t_shell *mini)
{
	handle_dollar(mini->tokens, mini);

	parse_commands(mini, mini->tokens);
	return (0);
}

void	handle_dollar(t_list *list, t_shell *mini)
{
	t_list	*current;
	char	*str;

	current = list;
	str = NULL;
	while (current)
	{
		str = ft_strchr(current->token, '$');
		if(str && !ft_is_in_single(current->token, str - current->token))
			process_token_expansion(current, mini);
		current = current->next;
	}
}

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

char	*expand_token(char *token, t_shell *mini)
{
	int		i;
	int		start;
	char	*result;
	char	*value;

	i = 0;
	start = 0;
	result = ft_strdup("");
	while(token && token[i])
	{
		if (token[i] == '$' && !ft_is_in_single(token, i))
		{
			result = string_build(result, ft_strnmdup(token, start, i - 1));
			i++;
			value = copy_var_value(mini, token, &i);
			result = string_build(result, value);
			start = i;
		}
		else
			i++;
	}
	if (token[start])
		result = string_build(result, token);
	return (result);
}

char	*copy_var_value(t_shell *mini, char *token, int *index)
{
	int start;
	char *key;
	char *value;

	start = *index;
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
	if ( !token[*index])
	{
		value = ft_strdup("$");
		return (value);
	}
	while (token[*index] && ((ft_isalnum(token[*index]) || token[*index] == '_')))
		(*index)++;
	key = ft_strnmdup(token, start, *index);
	value = extract_env_value(mini->initenv, key);
	if (!value)
		value = ft_strdup("");
	free(key);
	return (value);
}

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

static char	*string_build(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}
