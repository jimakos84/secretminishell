/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 02:10:17 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/29 09:24:00 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
 * Function declaration of helper fuctions
*/
int	check_filename(t_redir *r);
int	close_fds(int fd[][2], int limit);

/*
* Processes all redirections for a given command.
*
* For each redirection in the command's redir_list:
* - Validates the filename.
* - Handles the redirection based on its type (>, >>, <, <<).
* - Closes any open file descriptors after use.
*
* Parameters:
* - cmd: The command structure containing redirection information.
*
* Returns:
* - 0 on success, -1 if an error occurs.
*/

int	handle_redirections(t_cmd *cmd)
{
	t_redir	*r;

	if (!cmd || !cmd->redir_list)
		return (0);
	r = cmd->redir_list;
	while (r)
	{
		if (check_filename(r))
			return (-1);
		if (r->type == OPRD_CMD && handle_output(r, -1) < 0)
			return (-1);
		else if (r->type == APRD_CMD && handle_append(r, -1) < 0)
			return (-1);
		else if (r->type == IPRD_CMD && handle_input(r, -1) < 0)
			return (-1);
		else if (r->type == HDRD_CMD && handle_heredoc(r, -1) < 0)
			return (-1);
		r = r->next;
	}
	return (0);
}

/*
* Closes all file descriptors in a 2D array up to a given limit.
*
* Typically used to clean up pipe file descriptors.
*
* Parameters:
* - fd: A 2D array of file descriptors (each fd[i] is a pipe with 2 fds).
* - limit: The number of pipe pairs to close.
*
* Returns:
* - 0 after closing all descriptors.
*/

int	close_fds(int fd[][2], int limit)
{
	int	i;

	i = 0;
	while (i < limit)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
	return (0);
}

/*
* Checks if the filename in a redirection structure is valid (non-NULL).
*
* If the filename is missing, an error message is printed to stderr.
*
* Parameters:
* - r: The redirection structure.
*
* Returns:
* - 1 if the filename is invalid (NULL), 0 otherwise.
*/

int	check_filename(t_redir *r)
{
	if (!r->filename)
	{
		ft_putendl_fd("Redirection error: missing filename\n", 2);
		return (1);
	}
	return (0);
}
