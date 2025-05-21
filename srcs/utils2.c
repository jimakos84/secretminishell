/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:28:06 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/21 16:07:36 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/**
 * Function declaration
*/
int		quotes_checker(char *input, int len);
char	*ft_strjoin_free(char *s1, char *s2);
void	free_env(char **env);
bool	builtin_cmd(char *cmd);
bool	is_redir_or_pipe(char c);

/*
* Checks if there are any unmatched quotes in the input string up to
* a given length.
* This function supports both single and double quotes and returns
* the unmatched quote character if any.
*
* Parameters:
* - input: The input string to check.
* - len: The number of characters to check from the start.
*
* Returns:
* - 0 if all quotes are properly closed, or the unmatched quote
* character otherwise.
*/

int	quotes_checker(char *input, int len)
{
	int	in_quote;
	int	i;

	i = 0;
	in_quote = 0;
	while (input && input[i] && i < len)
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			if (in_quote == 0)
				in_quote = input[i];
			else if (in_quote == input[i])
				in_quote = 0;
		}
		i++;
	}
	return (in_quote);
}

/*
* Joins two strings and frees the first one.
*
* Useful for building strings dynamically while avoiding memory leaks.
*
* Parameters:
* - s1: The first string (will be freed).
* - s2: The second string.
*
* Returns:
* - A newly allocated string that is the concatenation of s1 and s2.
*/

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

/*
* Frees a NULL-terminated array of strings.
*
* Parameters:
* - env: The array of strings to be freed.
*/

void	free_env(char **env)
{
	int	i;

	i = 0;
	if (env)
	{
		while (env[i])
		{
			free(env[i]);
			i++;
		}
		free(env);
	}
}

/*
* Checks if a given command string matches one of the shell built-in commands.
*
* Recognized built-ins: exit, cd, env, pwd, unset, export, echo.
*
* Parameters:
* - cmd: The command string to check.
*
* Returns:
* - true if the command is a built-in, false otherwise.
*/

bool	builtin_cmd(char *cmd)
{
	static char	*builtins[8];
	int			i;

	i = 0;
	builtins[0] = "exit";
	builtins[1] = "cd";
	builtins[2] = "env";
	builtins[3] = "pwd";
	builtins[4] = "unset";
	builtins[5] = "export";
	builtins[6] = "echo";
	builtins[7] = NULL;
	while (builtins[i])
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}

/*
* Checks if a character is a redirection or pipe symbol.
*
* Valid symbols: '|', '>', '<'.
*
* Parameters:
* - c: The character to check.
*
* Returns:
* - true if the character is a redirection or pipe, false otherwise.
*/

bool	is_redir_or_pipe(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (true);
	else
		return (false);
}
