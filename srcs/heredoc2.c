/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:35:42 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/20 05:53:08 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int	handle_child_process(t_shell *mini, t_redir *r, int fd)
{
	int	status;

	status = 0;
	init_sig_heredoc();
	status = heredoc_interaction(mini, r, &fd);
	close(fd);
	exit (status);
}

int	handle_heredoc_status(int status, char *cache, int fd, t_redir *r)
{
	if (WEXITSTATUS(status))
	{
		close(fd);
		unlink(cache);
		free(cache);
		return (130);
	}
	close(fd);
	free(r->filename);
	r->filename = ft_strdup(cache);
	free(cache);
	return (0);
}
