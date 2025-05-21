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

void	heredoc_cleaner(t_shell *mini)
{
	clear_env(mini->initenv->env);
	free(mini->initenv);
	clear_and_exit(mini);
}

int	handle_child_process(t_shell *mini, t_redir *r, int fd, char *cache)
{
	int	status;

	init_sig_heredoc();
	status = heredoc_interaction(mini, r, &fd);
	heredoc_cleaner(mini);
	free(cache);
	close(fd);
	exit(status);
}

int	handle_heredoc_status(int status, char *cache, int fd, t_redir *r)
{
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
