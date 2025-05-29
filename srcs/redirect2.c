/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 02:33:04 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/29 09:24:20 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
 * Function declaration of helper fuctions
*/
int	handle_output(t_redir *r, int fd);
int	handle_append(t_redir *r, int fd);
int	handle_input(t_redir *r, int fd);
int	handle_heredoc(t_redir *r, int fd);

/*
* Handles output redirection ('>').
*
* Opens the specified file for writing (truncating existing content)
* and redirects STDOUT to it.
* Parameters:
* - r: The redirection structure containing the target filename.
* - fd: Temporary file descriptor (not used directly as input).
*
* Returns:
* - 0 on success, -1 on failure (with error message printed).
*/

int	handle_output(t_redir *r, int fd)
{
	fd = open(r->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
	{
		if (r->filename[0] == '\0')
		{
			ft_putendl_fd("No such file or directory", 2);
			return (-1);
		}
		ft_putendl_fd(" Permission denied", 2);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("Failed to redirect stdout");
		if (fd >= 0)
			close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/*
* Handles output append redirection ('>>').
*
* Opens the specified file for appending (creating it if necessary)
* and redirects STDOUT to it.
*
* Parameters:
* - r: The redirection structure containing the target filename.
* - fd: Temporary file descriptor (not used directly as input).
*
* Returns:
* - 0 on success, -1 on failure (with error message printed).
*/

int	handle_append(t_redir *r, int fd)
{
	fd = open(r->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd == -1)
	{
		if (r->filename[0] == '\0')
		{
			ft_putendl_fd("No such file or directory", 2);
			return (-1);
		}
		ft_putendl_fd(" Permission denied", 2);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("Failed to redirect stdout (append)");
		if (fd >= 0)
			close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/*
* Handles input redirection ('<').
*
* Opens the specified file for reading and redirects STDIN from it.
*
* Parameters:
* - r: The redirection structure containing the input filename.
* - fd: Temporary file descriptor (not used directly as input).
*
* Returns:
* - 0 on success, -1 on failure (with error message printed).
*/

int	handle_input(t_redir *r, int fd)
{
	struct stat	sb;

	fd = open(r->filename, O_RDONLY);
	if (fd == -1)
	{
		if (r->filename[0] == '\0' || stat(r->filename, &sb) == -1)
		{
			ft_putendl_fd(" No such file or directory", 2);
			return (-1);
		}
		ft_putendl_fd("Permission denied", 2);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("Failed to redirect stdin");
		if (fd >= 0)
			close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/*
* Handles here-document input redirection ('<<').
*
* Reads user input until a delimiter is matched and stores it in
* a temporary file.
* Then redirects STDIN to read from that file.
*
* Parameters:
* - r: The redirection structure containing the delimiter as filename.
* - fd: Temporary file descriptor (used internally).
*
* Returns:
* - 0 on success.
*/

int	handle_heredoc(t_redir *r, int fd)
{
	fd = open(r->filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		perror("dup2");
		return (-1);
	}
	close(fd);
	unlink(r->filename);
	return (0);
}
