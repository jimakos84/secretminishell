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
int	echo_error(char *msg, char *oldpwd);
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

static void	print_export(t_env *current)
{
	while (current)
	{
		printf("declare -x %s", current->name);
		if (current->value)
			printf("=\"%s\"", current->value);
		printf("\n");
		current = current->next;
	}
}

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
		print_export(current);
	while (args && args[i])
	{
		if (change_env_value(env, args[i++]))
			return (1);
	}
	return (0);
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

static int	export_validity(char *var)
{
	char	*equal_sign;
	size_t	name_len;

	equal_sign = ft_strchr(var, '=');
	if (equal_sign)
	{
		name_len = equal_sign - var;
		if (!is_valid_identifier_len(var, name_len))
			return (echo_error(" not a valid identifier", NULL));
	}
	else
	{
		name_len = ft_strlen(var);
		if (!is_valid_identifier_len(var, name_len))
			return (echo_error(" not a valid identifier", NULL));
	}
	return (0);
}

static int	modify_already_existing(t_env *current, char *var,
			char *equal_sign, size_t name_len)
{
	while (current)
	{
		if (ft_strncmp(var, current->name, name_len) == 0
			&& ft_strlen(current->name) == name_len)
		{
			if (equal_sign)
			{
				if (current->value)
					free(current->value);
				current->value = ft_strdup(equal_sign + 1);
			}
			return (0);
		}
		current = current->next;
	}
	return (1);
}

int	change_env_value(t_env *env, char *var)
{
	t_env	*current;
	char	*equal_sign;
	size_t	name_len;

	if (export_validity(var))
		return (1);
	equal_sign = ft_strchr(var, '=');
	if (equal_sign)
		name_len = equal_sign - var;
	else
		name_len = ft_strlen(var);
	current = env;
	if (modify_already_existing(current, var, equal_sign, name_len))
		add_to_list(&env, var);
	return (0);
}
