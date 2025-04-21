/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 12:22:57 by tsomacha          #+#    #+#             */
/*   Updated: 2024/12/06 19:11:37 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_get_newline(char **buffer, int c)
{
	char	*line;
	char	*new_buffer;
	ssize_t	index;

	line = NULL;
	new_buffer = NULL;
	index = ft_haschar(*buffer, c);
	if (!buffer || !*buffer || index < 0)
		return (line);
	line = ft_extract_line(*buffer, c);
	new_buffer = ft_strdup(&(*buffer)[index + 1]);
	free(*buffer);
	*buffer = new_buffer;
	return (line);
}

static int	ft_check_for_existing_newline(char **buffer, char **line)
{
	int	c;

	c = '\n';
	if (!buffer || !*buffer)
	{
		*line = NULL;
		return (0);
	}
	*line = ft_get_newline(buffer, c);
	if (!line || !*line)
	{
		*line = NULL;
		return (0);
	}
	return (1);
}

static int	ft_read_and_update_buffer(int fd, char **buffer, char **line)
{
	ssize_t	byte_read;
	char	*content;
	char	read_buffer[BUFFER_SIZE + 1];
	int		c;

	c = '\n';
	byte_read = 0;
	content = NULL;
	byte_read = read(fd, read_buffer, BUFFER_SIZE);
	if (byte_read <= 0)
	{
		free(content);
		return (0);
	}
	read_buffer[byte_read] = '\0';
	free(*line);
	*line = ft_strdup(read_buffer);
	content = ft_strjoin(*buffer, *line);
	free(*buffer);
	free(*line);
	*buffer = ft_strdup(content);
	free(content);
	*line = ft_get_newline(buffer, c);
	return (1);
}

static int	ft_remaining_line(char **buffer, char **line)
{
	if (!buffer || !*buffer)
		return (0);
	*line = ft_strdup(*buffer);
	free(*buffer);
	*buffer = NULL;
	return (1);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*holder_buffer;

	line = NULL;
	if (ft_check_for_existing_newline(&holder_buffer, &line))
		return (line);
	while (ft_read_and_update_buffer(fd, &holder_buffer, &line))
	{
		if (line)
			return (line);
	}
	if (ft_remaining_line(&holder_buffer, &line))
		return (line);
	free(holder_buffer);
	holder_buffer = NULL;
	return (line);
}
