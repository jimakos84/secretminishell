/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:13:11 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/08 11:34:37 by dvlachos         ###   ########.fr       */
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
		|| ft_strncmp(token, ">>", len) == 0
		|| ft_strncmp(token, "<<", len) == 0);
}

t_redir	*create_redir_node(int type, const char *filename)
{
	t_redir	*new;

	new = ft_calloc(1, sizeof(t_redir));
	if (!new)
		return (NULL);
	new->type = type;
	new->filename = ft_strdup(filename);
	remove_quotes_inplace(new->filename);
	new->next = NULL;
	return (new);
}

void	add_redir(t_redir **list, t_redir *new)
{
	t_redir	*temp;

	if (!*list)
	{
		*list = new;
		return ;
	}
	temp = *list;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

int	is_valid_identifier_len(const char *name, int len)
{
	int	i;

	if (len == 0 || !(ft_isalpha(name[0]) || name[0] == '_'))
		return (0);
	i = 1;
	while (i < len)
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}
