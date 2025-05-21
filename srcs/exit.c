/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:59:31 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/09 06:51:04 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int		check_valid_value(char *value);
int		handle_too_many_args(t_shell *mini);
int		validate_exit_argument(t_shell *mini, char *arg);
int		exit_mini(t_shell *mini);
void	exit_proccedure(t_shell *mini);

/*
* Normalizes the exit value to be within 0–255 range.
*
* Parameters:
* - value: The numeric string to convert and normalize.
*
* Returns:
* - The parsed and wrapped exit value.
*/
int	check_valid_value(char *value)
{
	int	exit_value;

	exit_value = ft_atoi(value);
	exit_value = exit_value % 256;
	return (exit_value);
}

/*
* Frees environment resources and shell context, then exits using
* the last recorded status.
*
* Parameters:
* - mini: Pointer to the shell context.
*/
void	exit_proccedure(t_shell *mini)
{
	int	status;

	status = 0;
	clear_env(mini->initenv->env);
	status = mini->initenv->last_status;
	free(mini->initenv);
	close(mini->_stdin);
	close(mini->_stdout);
	clear_and_exit(mini);
	rl_clear_history();
	exit (status);
}

/*
* Handles the case where `exit` is called with too many arguments.
*
* Sets an error status and prints a message, then exits.
*
* Parameters:
* - mini: Pointer to the shell context.
*
* Returns:
* - 1 if too many arguments were given, 0 otherwise.
*/

int	handle_too_many_args(t_shell *mini)
{
	if (mini->cmds->args[1] && mini->cmds->args[2])
	{
		if (!is_numerical(mini->cmds->args[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putendl_fd("numeric argument required", 2);
			mini->initenv->last_status = 2;
			exit_proccedure(mini);
			return (1);
		}
		mini->initenv->last_status = 1;
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putendl_fd("too many arguments", 2);
		return (1);
	}
	return (0);
}

/*
* Validates whether the argument to `exit` is numeric.
*
* If invalid, prints an error and exits. If valid, stores the exit value.
*
* Parameters:
* - mini: Pointer to the shell context.
* - arg: The argument to validate.
*
* Returns:
* - 1 if the argument is invalid (and the shell exits), 0 otherwise.
*/

int	validate_exit_argument(t_shell *mini, char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]) && arg[i] != '-' && arg[i] != '+')
		{
			mini->initenv->last_status = 2;
			printf("exit\n");
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(arg, 2);
			ft_putendl_fd(": numeric argument required", 2);
			exit_proccedure(mini);
			return (1);
		}
		i++;
	}
	mini->initenv->last_status = check_valid_value(arg);
	return (0);
}

/*
* Implements the `exit` built-in command.
*
* Behavior:
* - With no arguments: exits with the current status.
* - With one numeric argument: exits with that status.
* - With multiple arguments: prints an error and exits with status 1.
*
* Parameters:
* - mini: Pointer to the shell context.
*
* Returns:
* - 0 on normal exit, 1 if too many or invalid arguments are provided.
*/

int	exit_mini(t_shell *mini)
{
	if (handle_too_many_args(mini))
		return (1);
	if (mini->cmds->args[1])
	{
		if (validate_exit_argument(mini, mini->cmds->args[1]))
			return (1);
	}
	else
		printf("exit\n");
	cleanup_and_exit(mini);
	return (0);
}
