/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envrn.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:21:01 by dvlachos          #+#    #+#             */
/*   Updated: 2025/04/29 10:22:29 by dvlachos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

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
		node->value = ft_strdup("");
	node->next = NULL;
	return (node);
}

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

void	list_env(t_env **env, char **envp)
{
	while (envp && *envp)
		add_to_list(env, *envp++);
}

char	**copy_env(t_env *env)
{
	int		len;
	int		i;
	char	**copy;
	t_env	*tmp;

	tmp = env;
	len = ft_lst_len(env);
	copy = (char **)malloc((len + 1) * sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	while (tmp)
	{
		if (tmp->name)
		{
			if (tmp->value)
			{
				copy[i] = ft_strdup(tmp->name);
				copy[i] = ft_strjoin(copy[i], "=");
				copy[i] = ft_strjoin(copy[i], tmp->value);
			}
			else
				copy[i] = ft_strdup(tmp->name);
		}	
		tmp = tmp->next;
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

char	*extract_env_value(t_initenv *initenv, char *name)
{
	t_env	*temp;

	temp = initenv->env;
	while (temp)
	{
		if (ft_strncmp(name, temp->name, sizeof(name)) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return ("");
}

