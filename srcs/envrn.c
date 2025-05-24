/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envrn.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:21:01 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/24 06:34:58 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Function declaration of helper fuctions
*/
t_env	*new_node(char *content);
void	add_to_list(t_env **env, char *content);
void	list_env(t_env **env, char **envp);
char	**copy_env(t_env *env);
char	*extract_env_value(t_initenv *initenv, char *name);

/*
* Creates a new environment variable node from a "KEY=VALUE" string.
*
* Parameters:
* - content: The environment string to parse.
*
* Returns:
* - A pointer to the newly created t_env node, or NULL on failure.
*/

t_env	*new_node(char *content)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->name = ft_substr(content, 0, (ft_strchr(content, '=') - content));
	if (!node->name)
		return (NULL);
	if (ft_strchr(content, '='))
	{
		node->value = ft_strdup(ft_strchr(content, '=') + 1);
		if (!node->value)
			return (NULL);
	}
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

/*
* Adds a new environment variable node to the end of the environment list.
*
* Parameters:
* - env: Pointer to the head of the environment list.
* - content: The "KEY=VALUE" string to be added.
*/

void	add_to_list(t_env **env, char *content)
{
	t_env	*new;
	t_env	*current;

	new = new_node(content);
	if (!new)
		return ;
	if (*env == NULL)
		*env = new;
	else
	{
		current = *env;
		while (current && current->next)
			current = current->next;
		current->next = new;
	}
}

/*
* Builds a linked list of environment variables from an array of strings.
*
* Parameters:
* - env: Pointer to the environment list to populate.
* - envp: Array of "KEY=VALUE" strings (typically from main).
*/

void	list_env(t_env **env, char **envp)
{
	if (!*envp)
	{
		add_to_list(env, "SHLVL=1");
		add_to_list(env, "_=/usr/bin/env");
	}
	while (envp && *envp)
		add_to_list(env, *envp++);
}

/*
* Creates a deep copy of the environment list as a NULL-terminated
* array of strings.
*
* Each string is in the format "KEY=VALUE".
*
* Parameters:
* - env: Pointer to the head of the environment list.
*
* Returns:
* - A newly allocated array of environment strings, or NULL on failure.
*/

char	**copy_env(t_env *env)
{
	int		i;
	char	**copy;
	t_env	*tmp;

	tmp = env;
	copy = (char **)malloc(((ft_lst_len(env)) + 1) * sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	while (tmp)
	{
		if (tmp->name)
		{
			copy[i] = ft_strdup(tmp->name);
			if (tmp->value)
			{
				copy[i] = string_build(copy[i], "=");
				copy[i] = string_build(copy[i], tmp->value);
			}
		}
		tmp = tmp->next;
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

/*
* Retrieves the value of a named environment variable from the initenv struct.
*
* Parameters:
* - initenv: Pointer to the initialized environment structure.
* - name: The name of the environment variable to retrieve.
*
* Returns:
* - The value associated with the variable name, or an empty string if not found.
*/

char	*extract_env_value(t_initenv *initenv, char *name)
{
	t_env	*temp;
	char	*value;

	value = NULL;
	temp = initenv->env;
	while (temp)
	{
		if (ft_strcmp(name, temp->name) == 0)
		{
			value = temp->value;
			break ;
		}
		temp = temp->next;
	}
	if (!value)
		value = "";
	return (value);
}
