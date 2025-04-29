/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:13:11 by dvlachos          #+#    #+#             */
/*   Updated: 2025/04/29 10:34:53 by dvlachos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int	ft_lst_len(t_env *env)
{
	t_env	*temp;
	int		len;

	len = 0;
	temp = env;
	while (temp)
	{
		len++;
		temp = temp->next;
	}
	return (len);
}

bool	is_invalid_pipe_sequence(t_list *current)
{
	if (!current || !current->next)
		return (false);
	if (contains_unquoted_char(current->token, '|')
		&& contains_unquoted_char(current->next->token, '|'))
		return (true);
	return (false);
}

int	is_redirection_token(char *token)
{
	size_t	len;

	len = ft_strlen(token);
	return (ft_strncmp(token, ">", len) == 0
		|| ft_strncmp(token, "<", len) == 0
		|| ft_strncmp(token, ">>", len) == 0);
}
