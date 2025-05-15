/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:13:40 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/14 04:17:29 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
 * Function declaration of helper fuctions
*/
t_cmd	*allocate_cmd_node(void);
char	**allocate_args_array(int num_args);
char	*set_path_name(t_shell *mini, char *token);
char	*copy_var_value(t_shell *mini, char *token, int *index);
char	*expand_token(char *token, t_shell *mini);

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
	int		i;

	i = 0;
	args = malloc(sizeof(char *) * (num_args + 1));
	if (!args)
		return (NULL);
	while (i <= num_args)
		args[i++] = NULL;
	return (args);
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
	char	*temp;
	char	*full_path;
	int		i;

	i = 0;
	if (!token || !mini->initenv)
		return (ft_strdup(token));
	path_dirs = ft_split(extract_env_value(mini->initenv, "PATH"), ':');
	if (!path_dirs)
		return (ft_strdup(token));
	while (path_dirs[i])
	{
		temp = string_build(ft_strdup(path_dirs[i]), "/");
		full_path = string_build(temp, token);
		if (access(full_path, F_OK) == 0 && token[0] && token[0] != '.')
		{
			clear_path_dirs(path_dirs, i);
			return (full_path);
		}
		free(full_path);
		free(path_dirs[i++]);
	}
	free(path_dirs);
	return (ft_strdup(token));
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
	value = ft_strdup(extract_env_value(mini->initenv, key));
	if (!value || value[0] == '\0')
	{
		if (value)
			free(value);
		value = NULL;
	}
	free(key);
	return (value);
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
	result = strdup("");
	while (token && token[i])
	{
		if (token[i] == '$')
		{
			result = string_build2(result, ft_strnmdup(token, start, i));
			i++;
			value = copy_var_value(mini, token, &i);
			result = string_build2(result, value);
			start = i;
		}
		else
			i++;
	}
	if (token[start])
		result = string_build2(result, ft_strnmdup(token, start, i));
	return (result);
}
