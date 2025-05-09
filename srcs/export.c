/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:57:12 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/09 03:40:03 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Function declaration of helper fuctions
*/
int	echo_error(char *msg);
int	change_env_value(t_env *env, char *var);

/*
* Implements the 'export' built-in command.
*
* - If no arguments are provided, prints all environment variables
* in `declare -x` format.
* - If arguments are provided, updates or adds the variables to
* the environment list.
*
* Parameters:
* - mini: Pointer to the shell structure containing the environment
* and command arguments.
*
* Returns:
* - 0 on success, 1 if any variable is invalid.
*/

int	builtin_export(t_shell *mini)
{
	t_env	*env;
	t_env	*current;
	char	**args;
	int		i;

	i = 1;
	args = mini->cmds->args;
	env = mini->initenv->env;
	current = env;
	if (!args[i])
	{
		while (current)
		{
			printf("declare -x %s=\"%s\"\n", current->name, current->value);
			current = current->next;
		}
	}
	while (args && args[i])
	{
		if (change_env_value(env, args[i++]))
			return (1);
	}
	return (0);
}

/*
* Prints an error message to stderr and returns 1.
*
* Parameters:
* - msg: The error message to display.
*
* Returns:
* - Always returns 1.
*/

int	echo_error(char *msg)
{
	ft_putendl_fd(msg, 2);
	return (1);
}

/*
* Updates the value of an existing environment variable,
* or adds it if it doesn't exist.
*
* - Validates the identifier before processing.
* - If the variable exists, updates its value.
* - Otherwise, adds a new node to the environment list.
*
* Parameters:
* - env: The environment variable list.
* - var: The variable in "KEY=VALUE" or "KEY" format.
*
* Returns:
* - 0 on success, 1 on invalid identifier.
*/

int	change_env_value(t_env *env, char *var)
{
	t_env	*current;
	char	*equal_sign;

	equal_sign = ft_strrchr(var, '=');
	if (equal_sign)
	{
		if (!is_valid_identifier_len(var, (equal_sign - var)))
			return (echo_error("not a valid identifier"));
	}
	else if (!is_valid_identifier_len(var, ft_strlen(var)))
		return (echo_error("not a valid identifier"));
	current = env;
	while (current)
	{
		if (ft_strncmp(var, current->name, ft_strlen(var)) == 0)
		{
			free(current->value);
			current->value = ft_strdup(ft_strchr(var, '=') + 1);
			return (0);
		}
		current = current->next;
	}
	add_to_list(&env, var);
	return (0);
}
