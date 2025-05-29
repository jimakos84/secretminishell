/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:23:57 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/29 07:53:39 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Function declaration of helper fuctions
*/
int		syntax_error(char *input, int code);
void	check_stat(char *path, t_shell *mini, int **fd);
void	perror_exit(const char *msg);
void	stat_error(char *path, char c, t_shell *mini, int **fd);
void	p_exe_error(char *command, int err, t_initenv *initenv);

/*
* Prints a syntax error message and frees the input string.
*
* Combines a standard prefix with a custom message and prints to stderr.
*
* Parameters:
* - input: The input string to be freed.
* - msg: The token or message to append to the standard syntax error.
* - code: The error code to return.
*
* Returns:
* - The provided error code.
*/

int	syntax_error(char *input, int code)
{
	ft_putendl_fd("minishell: syntax error near unexpected token", 2);
	free(input);
	return (code);
}

/*
* Checks file existence, executability, and whether it is a directory.
*
* If the file doesn't exist or is not executable, prints an error and exits with:
* - 127 if not found or command not found.
* - 126 if it is a directory or has no execution permission.
*
* Parameters:
* - path: The file path to check.
*/

void	check_stat(char *path, t_shell *mini, int **fd)
{
	struct stat	sb;

	if (!path)
		return ;
	if (stat(path, &sb) == -1)
	{
		ft_putstr_fd(path, 2);
		if (!ft_strchr(path, '/'))
			ft_putendl_fd(": command not found", 2);
		else
			ft_putendl_fd(": No such file or directory", 2);
		free(mini->pids);
		free_fds(fd, mini->num_cmds - 1);
		clear_env(mini->initenv->env);
		free(mini->initenv);
		clear_and_exit(mini);
		exit(127);
	}
	else if (S_ISDIR(sb.st_mode))
		stat_error(path, 's', mini, fd);
	else if (access(path, X_OK) == -1)
		stat_error(path, 'a', mini, fd);
}

/*
* Prints an error message using `perror` and exits the program.
*
* Parameters:
* - msg: The message prefix to pass to `perror`.
*/

void	perror_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

/*
* Prints an error message based on file path and error type, then exits.
*
* Handles:
* - 's': Directory error → exits with 126.
* - 'a': Permission denied → exits with 126.
* - Missing '/' in path is treated as command not found → exits with 127.
*
* Parameters:
* - path: The path causing the error.
* - c: The error type ('s' or 'a').
*/

void	stat_error(char *path, char c, t_shell *mini, int **fd)
{
	ft_putstr_fd(path, 2);
	if (!ft_strchr(path, '/'))
	{
		ft_putendl_fd(": command not found", 2);
		free_fds(fd, mini->num_cmds - 1);
		free(mini->pids);
		clear_env(mini->initenv->env);
		free(mini->initenv);
		clear_and_exit(mini);
		exit (127);
	}
	else
	{
		if (c == 's')
			ft_putendl_fd(": Is a directory", 2);
		if (c == 'a')
			ft_putendl_fd(": Permission denied", 2);
		free(mini->pids);
		free_fds(fd, mini->num_cmds - 1);
		clear_env(mini->initenv->env);
		free(mini->initenv);
		clear_and_exit(mini);
		exit(126);
	}
}

/*
* Sets the shell's exit status and prints an appropriate error message
* based on exec failure.
*
* Handles:
* - ENOENT: Command not found (exit status 127).
* - EACCES: Permission or directory access error (exit status 126).
* - Other: Generic error with exit status as (err % 256).
*
* Parameters:
* - command: The command that failed.
* - err: The error number (e.g., from `errno`).
* - initenv: Pointer to the shell's environment structure.
*/

void	p_exe_error(char *command, int err, t_initenv *initenv)
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
	else if (err == ENOEXEC)
		initenv->last_status = 0;
	else
		initenv->last_status = err % 256;
}
