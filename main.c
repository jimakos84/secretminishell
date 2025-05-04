/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 09:30:51 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/04 04:29:23 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/shell.h"

/*
* Globle varibale to handle singnal
*/
volatile sig_atomic_t	g_sig = 0;

/*
* Decalarion of helper functions
*/
static void	init_env(t_initenv **initenv, char **envp);
static void	init_shell(int status, t_initenv *initenv);

/*
 * Entry point of the Minishell program.
 * - Ignores the program arguments (`av`)
 * - Initializes signal handlers using `init_sig`.
 * - Allocates and prepares the environment data structure via `init_env`.
 * - Continuously reads and processes shell input using `init_shell`.
 * - Returns the last exit status recorded in the environment structure.
 *
 * Parameters:
 * - ac: Argument count.
 * - av: Argument vector (unused).
 * - envp: Environment variable array passed from the system.
 *
 * Returns:
 * - The last exit status from the shell loop.
 */
int	main(int ac, char **av, char **envp)
{
	t_initenv	*initenv;

	initenv = NULL;
	(void)av;
	if (ac == 1)
	{
		init_sig();
		init_env(&initenv, envp);
		while (1)
			init_shell(initenv->last_status, initenv);
	}
	return (initenv->last_status);
}

/*
* Initializes the initenv structure with environment variables and other fields.
* - Allocates memory for the t_initenv structure.
* - Initializes the fields copy_env, env, and home to NULL.
* - Sets last_status to 0.
* - Calls list_env to parse envp into a linked list of environment variables.
* - Extracts HOME variable and stores a duplicated string in home.
*
* Parameters:
* - initenv: A double pointer to t_initenv
* - envp: 2D array of strings containing the envp passed to main.
*/
void	init_env(t_initenv **initenv, char **envp)
{
	*initenv = malloc(sizeof(t_initenv));
	(*initenv)->copy_env = NULL;
	(*initenv)->env = NULL;
	(*initenv)->home = NULL;
	(*initenv)->last_status = 0;
	list_env(&(*initenv)->env, envp);
	(*initenv)->home = ft_strdup(extract_env_value(*initenv, "HOME"));
}

/*
* Initializes a single iteration of the shell input loop.
* - Displays the "minishell> " prompt and reads user input.
* - Exits the shell if the input is NULL (e.g., user pressed Ctrl+D).
* - Ignores and frees the input if it is empty (whitespace only).
* - Adds valid input to the history.
* - Passes the input to the shell activation logic (activate_shell).
*
* Parameters:
* - status: An integer representing the last command's exit status
* - initenv: A pointer to the initialized environment structure
*/
static void	init_shell(int status, t_initenv *initenv)
{
	char	*input;

	(void)status;
	input = readline("minishell> ");
	if (!input)
		exit (1);
	if (ft_isempty(input))
	{
		free(input);
		return ;
	}
	add_history(input);
	status = activate_shell(input, initenv);
	free(input);
}
