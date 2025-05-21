/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:03:27 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/09 05:16:31 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Function declaration of helper fuctions
*/
void	remove_env_nodes(t_shell *mini, char *unset);
int		builtin_unset(t_shell *mini);
int		builtin_env(t_shell *mini);
int		builtin_pwd(t_initenv *env);

/*
* Removes a node from the environment list that matches the given variable name.
*
* Parameters:
* - mini: Pointer to the shell context containing the environment list.
* - unset: The name of the variable to remove.
*/

void	remove_env_nodes(t_shell *mini, char *unset)
{
	t_env	*current;
	t_env	*prev;

	current = mini->initenv->env;
	prev = NULL;
	if (!unset)
		return ;
	while (current)
	{
		if (ft_strncmp(current->name, unset, ft_strlen(unset)) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				mini->initenv->env = current->next;
			free(current->name);
			if (current->value)
				free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

/*
* Implements the `unset` built-in command.
*
* Iterates over arguments and removes matching environment variables.
*
* Parameters:
* - mini: Pointer to the shell context.
*
* Returns:
* - 0 on success, 1 if argument list is empty.
*/

int	builtin_unset(t_shell *mini)
{
	int		i;
	char	**unset;

	unset = mini->cmds->args;
	i = 1;
	if (!unset)
		return (1);
	while (unset[i])
		remove_env_nodes(mini, unset[i++]);
	return (0);
}

/*
* Implements the `env` built-in command.
*
* Prints the current environment variables in the format NAME=VALUE.
*
* Parameters:
* - mini: Pointer to the shell context.
*
* Returns:
* - 0 if no extra arguments are passed and environment is printed.
* - 1 if unexpected arguments are provided.
*/

int	builtin_env(t_shell *mini)
{
	t_env	*temp;

	temp = mini->initenv->env;
	if (!mini->cmds->args[1])
	{
		while (temp)
		{
			if (temp->value && temp->value[0])
			{
				printf("%s", temp->name);
				printf("=");
				printf("%s\n", temp->value);
			}
			temp = temp->next;
		}
		return (0);
	}
	else
		return (1);
}

/*
* Implements the `pwd` built-in command.
*
* Prints the current working directory to stdout.
*
* Returns:
* - 0 on success.
*/

int	builtin_pwd(t_initenv *env)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		pwd = ft_strdup(extract_env_value(env, "PWD"));
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
