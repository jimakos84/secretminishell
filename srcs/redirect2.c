/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 02:33:04 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/09 02:39:49 by tsomacha         ###   ########.fr       */
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

static int	null_heredoc(char *filename)
{
	ft_putstr_fd("minishell: warning: here-document"
		" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(filename, 2);
	ft_putendl_fd("')", 2);
	return (0);
}

int	handle_heredoc(t_redir *r, int fd)
{
	char	*pmpt;

	pmpt = NULL;
	fd = open(CACHE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	pmpt = readline(">");
	if (!pmpt)
		return (null_heredoc(r->filename));
	while (pmpt || ft_isempty(pmpt))
	{
		if (ft_strcmp(r->filename, pmpt) == 0)
		{
			free(pmpt);
			break ;
		}
		write(fd, pmpt, ft_strlen(pmpt));
		write(fd, "\n", 2);
		free(pmpt);
		pmpt = readline(">");
		if (!pmpt)
			return (null_heredoc(r->filename));
	}
	close(fd);
	fd = open(CACHE, O_RDONLY);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}
