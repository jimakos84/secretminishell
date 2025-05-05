/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:23:57 by dvlachos          #+#    #+#             */
/*   Updated: 2025/04/29 10:24:09 by dvlachos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int	syntax_error(char *input, char *msg, int code)
{
	char	*std_msg;
	char	*final;

	std_msg = "minishell: syntax error near unexpected token ";
	final = ft_strjoin(std_msg, msg);
	ft_putendl_fd(final, 2);
	free(final);
	free(input);
	return (code);
}

void p_exe_error(char *command, int err, t_initenv *initenv)
{
    if (err == ENOENT)
    {
        initenv->last_status = 127;
        ft_putstr_fd(command, 2);
        ft_putendl_fd(": command not found", 2);
    }
    else if (err == EACCES)
    {
        initenv->last_status = 126;
        ft_putstr_fd(command, 2);
        ft_putendl_fd(": Is a directory", 2);
    }
    else
    {
        // Ensure exit code is in range 0-255
        initenv->last_status = err % 256;
    }
}

void	perror_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
