/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 11:46:37 by dvlachos          #+#    #+#             */
/*   Updated: 2025/02/05 10:57:59 by dvlachos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*new_leftovers(char *left_overs)
{
	int		i;
	int		j;
	char	*new_left;

	i = 0;
	while (left_overs[i] && left_overs[i] != '\n')
		i++;
	if (!left_overs[i])
	{
		free(left_overs);
		return (NULL);
	}
	new_left = (char *)malloc(sizeof(char) * (ft_strlen(left_overs) - i + 1));
	if (!new_left)
		return (NULL);
	i++;
	j = 0;
	while (left_overs[i])
		new_left[j++] = left_overs[i++];
	new_left[j] = '\0';
	free(left_overs);
	return (new_left);
}

static char	*clean_line_func(char *found_newline)
{
	int		i;
	char	*str;

	i = 0;
	if (!found_newline[i])
		return (NULL);
	while (found_newline[i] && found_newline[i] != '\n')
		i++;
	str = (char *)malloc(sizeof(char) * (i + 2));
	if (!str)
		return (NULL);
	i = 0;
	while (found_newline[i] && found_newline[i] != '\n')
	{
		str[i] = found_newline[i];
		i++;
	}
	if (found_newline[i] == '\n')
	{
		str[i] = found_newline[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static char	*get_newline(int fd, char *left_overs)
{
	static char	stash[BUFFER_SIZE + 1];
	char		*found_newline;
	char		*temp;
	int			bytesread;

	found_newline = ft_strdup(left_overs);
	if (!found_newline)
		return (NULL);
	bytesread = 1;
	while (!ft_strchr(found_newline, '\n') && bytesread > 0)
	{
		bytesread = read(fd, stash, BUFFER_SIZE);
		if (bytesread < 0)
		{
			free(found_newline);
			return (NULL);
		}
		stash[bytesread] = '\0';
		temp = ft_strjoin(found_newline, stash);
		free(found_newline);
		found_newline = temp;
		if (!found_newline)
			return (NULL);
	}
	return (found_newline);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*clean_line;
	static char	*left_overs;
	char		*temp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (left_overs)
		temp = left_overs;
	else
		temp = "";
	line = get_newline(fd, temp);
	if (!line)
	{
		free(left_overs);
		left_overs = NULL;
		return (NULL);
	}
	clean_line = clean_line_func(line);
	free(left_overs);
	left_overs = new_leftovers(line);
	return (clean_line);
}
