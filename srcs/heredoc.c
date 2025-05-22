/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 02:37:52 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/22 03:52:20 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

extern int	g_sig;

char	*expand_pmpt(t_shell *mini, char *pmpt, t_redir *r)
{
	char	*expanded;

	if (ft_strchr(pmpt, '$') && !r->was_quoted)
	{
		expanded = expand_token(pmpt, mini);
		free(pmpt);
	}
	else
	{
		expanded = ft_strdup(pmpt);
		free(pmpt);
	}
	return (expanded);
}

static int	null_heredoc(char *filename)
{
	if (g_sig == SIGINT)
		return (130);
	ft_putstr_fd("minishell: warning: here-document"
		" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(filename, 2);
	ft_putendl_fd("')", 2);
	return (0);
}

int	heredoc_interaction(t_shell *mini, t_redir *r, int *fd)
{
	char	*pmpt;

	while (true)
	{
		pmpt = readline(">");
		if (!pmpt)
		{
			close(*fd);
			unlink(r->filename);
			return (null_heredoc(r->filename));
		}
		if (ft_strcmp(r->filename, pmpt) == 0)
		{
			free(pmpt);
			break ;
		}
		pmpt = expand_pmpt(mini, pmpt, r);
		write(*fd, pmpt, ft_strlen(pmpt));
		write(*fd, "\n", 1);
		free(pmpt);
	}
	return (0);
}

int	execute_heredoc(t_shell *mini, t_redir *r, int fd)
{
	pid_t	pid;
	int		status;
	char	*cache;

	cache = set_cache_file_name();
	fd = open(cache, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd < 0)
		return (perror("open"), -1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), close(fd), -1);
	else if (pid == 0)
		handle_child_process(mini, r, fd, cache);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, SIG_DFL);
	if (!mini->cmds->cmd)
		unlink(cache);
	return (handle_heredoc_status(status, cache, fd, r));
}

int	preprocessing_heredocs(t_shell *mini)
{
	t_cmd	*current;
	t_redir	*node;
	int		status;

	status = 0;
	current = mini->cmds;
	while (current)
	{
		if (current->redir_list)
		{
			node = current->redir_list;
			while (node)
			{
				if (node->type == 5)
				{
					status = execute_heredoc(mini, node, -1);
					if (status)
						return (status);
				}
				node = node->next;
			}
		}
		current = current->next;
	}
	return (0);
}
