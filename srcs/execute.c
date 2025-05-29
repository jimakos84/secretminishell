/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:24:34 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/29 04:14:31 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

extern int	g_sig;
/*
* Function declaration of helper fuctions
*/
int		execute(t_shell *mini);
int		**alloc_fds(int limit);
int		init_pipes(int **fd, int limit);
int		handle_builtin(t_shell *mini, t_cmd *current);
int		wait_for_children(int n, t_shell *mini, pid_t *pids);

/*
* Main execution function for the shell.
*
* Handles:
* - Single built-in commands (without forking).
* - Multi-command pipelines using fork/exec and pipes.
*
* Parameters:
* - mini: Shell context.
*
* Returns:
* - Exit status of the last command.
*/

int	execute(t_shell *mini)
{
	int		index;
	int		**fd;
	t_cmd	*current;
	pid_t	*pids;
	int		limit;

	if (mini->num_cmds > 1)
		limit = mini->num_cmds;
	else
		limit = 1;
	current = mini->cmds;
	index = 0;
	if (mini->num_cmds == 1 && current->is_builtin)
		return (handle_builtin(mini, current));
	fd = alloc_fds(limit - 1);
	if (init_pipes(fd, limit - 1))
		return (1);
	pids = run_commands(mini, current, fd, &index);
	if (!pids)
		return (0);
	close_fds2(fd, limit - 1);
	wait_for_children(index, mini, pids);
	free_fds(fd, limit - 1);
	free(pids);
	return (mini->initenv->last_status);
}

/*
* Allocates memory for a 2D array of pipe file descriptors.
*
* Parameters:
* - limit: Number of pipe pairs to allocate.
*
* Returns:
* - Pointer to the allocated fd array, or NULL on failure.
*/

int	**alloc_fds(int limit)
{
	int	**fd;
	int	i;

	fd = ft_calloc(sizeof(int *), limit);
	if (!fd)
		return (NULL);
	i = 0;
	while (i < limit)
	{
		fd[i] = ft_calloc(sizeof(int), 2);
		if (!fd[i])
		{
			free_fds(fd, i);
			return (NULL);
		}
		i++;
	}
	return (fd);
}

/*
* Initializes pipes by calling `pipe()` on each pair.
*
* Parameters:
* - fd: Array of pipe fd pairs.
* - limit: Number of pipes to create.
*
* Returns:
* - 0 on success, 1 on error.
*/

int	init_pipes(int **fd, int limit)
{
	int	i;

	i = 0;
	while (fd && i < limit)
	{
		if (pipe(fd[i]) == -1)
		{
			perror("Pipe creation failed");
			while (--i >= 0)
			{
				if (close(fd[i][0]) == -1)
					perror("close fd[0] fails");
				if (close(fd[i][1]) == -1 )
					perror("close fd[1] fails");
			}
			free_fds(fd, limit);
			return (1);
		}
		i++;
	}
	return (0);
}

/*
* Handles execution of a single built-in command without forking.
* Saves and restores original stdin/stdout.
*
* Parameters:
* - mini: Shell context.
* - current: Command to execute.
*
* Returns:
* - The built-in’s return value.
*/

int	handle_builtin(t_shell *mini, t_cmd *current)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	mini->_stdin = saved_stdin;
	mini->_stdout = saved_stdout;
	if (saved_stdin == -1 || saved_stdout == -1)
		perror_exit("dup failed");
	if (handle_redirections(current) == -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}
	mini->initenv->last_status = check_builtin(current, mini);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (mini->initenv->last_status);
}

/*
* Waits for all child processes to finish and collects their exit statuses.
*
* The last command's status becomes the shell’s last_status.
*
* Parameters:
* - count: Number of child processes.
* - initenv: Pointer to the environment state.
* - pids: Array of child PIDs.
*
* Returns:
* - Exit status of the last command.
*/

int	wait_for_children(int n, t_shell *mini, pid_t *pids)
{
	int	i;
	int	status;
	int	last_exit_status;

	i = 0;
	last_exit_status = 0;
	signal(SIGINT, SIG_IGN);
	while (i < n)
	{
		if (waitpid(pids[i], &status, 0) == -1)
			continue ;
		if (i == n - 1)
		{
			if (WIFEXITED(status))
				last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_exit_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	signal(SIGINT, handler);
	last_exit_status = check_heredoc_no_cmd(mini, last_exit_status);
	mini->initenv->last_status = last_exit_status;
	return (last_exit_status);
}
