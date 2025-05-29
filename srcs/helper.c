/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 05:28:10 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/29 05:24:46 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

extern int	g_sig;
/*
 * Function declaration of helper fuctions
 */
static t_shell	*configure_mini_shell(t_initenv *env);

/*
 * Handles the main logic of processing a single shell input command.
 * - Allocates and initializes a t_shell struct to manage the current
 * shell state.
 * - Configures the shell structure with environment data.
 * - Validates the input string for syntax or structural issues.
 * - Tokenizes the input into a linked list of shell tokens.
 * - Parses and expands variables in the token list.
 * - Executes the parsed command(s).
 * - Cleans up and handles shell exit conditions.
 * If any step fails, the function returns 1 to indicate an error.
 *
 * Parameters:
 * - input: The command line input string entered by the user.
 * - env: A pointer to the initialized environment structure.
 *
 * Returns:
 * - 0 on success, or 1 if an error occurs during any stage.
 */
static int	free_mini_and_return(t_shell *mini, int status)
{
	if (mini->initenv->last_status == 139)
	{
		ft_putstr_fd("☠️ minishell: segmentation fault (CORE DUMPED)☠️  ", 2);
		ft_putendl_fd(mini->cmds->command, 2);
	}
	init_sig();
	clear_and_exit(mini);
	return (status);
}

int	activate_shell(char *input, t_initenv *env)
{
	int		status;
	t_shell	*mini;

	mini = configure_mini_shell(env);
	if (!mini)
		return (1);
	status = input_validate(&input, env);
	if (status)
		return (free_mini_and_return(mini, status));
	status = extract_tokens(&mini->tokens, input);
	if (status)
		return (free_mini_and_return(mini, status));
	status = parse_and_expand(mini);
	if (status)
		return (free_mini_and_return(mini, status));
	status = preprocessing_heredocs(mini);
	if (status)
		return (free_mini_and_return(mini, status));
	status = execute(mini);
	if (status)
		return (free_mini_and_return(mini, status));
	status = clear_and_exit(mini);
	if (status)
		return (status);
	return (0);
}

/*
 * Initializes the fields of a t_shell structure for a new shell session.
 *
 * Parameters:
 * - mini: A double pointer to the t_shell structure to configure.
 * - env: A pointer to the initialized environment structure to link with
 * the shell.
 */
static t_shell	*configure_mini_shell(t_initenv *env)
{
	t_shell	*mini;

	mini = malloc(sizeof(t_shell));
	if (!mini)
	{
		perror("mini initializing fails!");
		return (NULL);
	}
	mini->num_cmds = 0;
	mini->tokens = NULL;
	mini->cmds = NULL;
	mini->initenv = env;
	mini->status = 0;
	mini->cmds = NULL;
	mini->_stdin = -1;
	mini->_stdout = -1;
	mini->pids = NULL;
	return (mini);
}
