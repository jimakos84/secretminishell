/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:35:42 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/18 15:36:33 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int	handle_child_process(t_shell *mini, t_redir *r, int fd)
{
	int	status;

	init_sig_heredoc();
	status = heredoc_interaction(mini, r, &fd);
	close(fd);
	exit(status);
}

int	handle_heredoc_status(int status, char *cache, int fd, t_redir *r)
{
	if (WIFSIGNALED(status))
	{
		close(fd);
		unlink(cache);
		free(cache);
		return (130);
	}
	if (WEXITSTATUS(status) != 0)
	{
		close(fd);
		unlink(cache);
		free(cache);
		return (WEXITSTATUS(status));
	}
	close(fd);
	free(r->filename);
	r->filename = ft_strdup(cache);
	free(cache);
	return (0);
}
