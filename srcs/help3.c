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
