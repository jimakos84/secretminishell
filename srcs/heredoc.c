/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 02:37:52 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/16 05:24:34 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

static int	null_heredoc(char *filename)
{
	ft_putstr_fd("minishell: warning: here-document"
		" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(filename, 2);
	ft_putendl_fd("')", 2);
	return (0);
}

int	heredoc_interaction(t_redir *r, int *fd, char *pmpt)
{
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
		write(*fd, pmpt, ft_strlen(pmpt));
		write(*fd, "\n", 1);
		free(pmpt);
		pmpt = readline(">");
		if (!pmpt)
			return (null_heredoc(r->filename));
	}
	return (0);
}

char	*set_cache_file_name(void)
{
	char	*name;

	name = string_build2(ft_itoa(getpid()), ft_strdup(".dts.cache"));
	if (!name)
		return (NULL);
	return (name);
}
