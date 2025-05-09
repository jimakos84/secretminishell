/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 05:28:10 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/09 03:12:40 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
 * Function declaration of helper fuctions
 */
static void	configure_mini_shell(t_shell **mini, t_initenv *env);

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
int	activate_shell(char *input, t_initenv *env)
{
	t_shell	*mini;
	int		status;

	mini = malloc(sizeof(t_shell));
	if (!mini)
		return (1);
	configure_mini_shell(&mini, env);
	status = input_validate(&input);
	if (status)
		return (status);
	status = extract_tokens(&mini->tokens, input);
	if (status)
		return (status);
	status = parse_and_expand(mini);
	if (status)
		return (status);
	status = execute(mini);
	if (status)
		return (status);
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
static void	configure_mini_shell(t_shell **mini, t_initenv *env)
{
	(*mini)->num_cmds = 0;
	(*mini)->tokens = NULL;
	(*mini)->cmds = NULL;
	(*mini)->initenv = env;
	(*mini)->status = 0;
	(*mini)->cmds = NULL;
}
