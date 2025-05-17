/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:26:31 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/13 17:27:15 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

char	*string_build2(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	return (joined);
}

char	**get_path_values(t_shell *mini)
{
	char	*value;
	char	**dirs;

	value = extract_env_value(mini->initenv, "PATH");
	if (!value || value[0] == '\0')
		return (NULL);
	dirs = ft_split(value, ':');
	if (!dirs)
	{
		free(value);
		return (NULL);
	}
	free(value);
	return (dirs);
}

bool	is_numerical(char *str)
{
	int		i;

	i = 0;
	if (!str)
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	check_heredoc_no_cmd(t_shell *mini, int last_exit_status)
{
	if (mini)
	{
		if (mini->cmds)
		{
			if (!mini->cmds->command && mini->cmds->redir_list
				&& mini->cmds->redir_list->type == 5)
				return (0);
			else
				return (last_exit_status);
		}
	}
	return (0);
}
