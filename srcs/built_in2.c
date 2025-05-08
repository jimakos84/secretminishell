/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:03:27 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/08 11:32:18 by dvlachos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

static void	remove_env_nodes(t_shell *mini, char *unset)
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

void	builtin_unset(t_shell *mini)
{
	int		i;
	char	**unset;

	unset = mini->cmds->args;
	i = 1;
	if (!unset)
		return ;
	while (unset[i])
		remove_env_nodes(mini, unset[i++]);
}
