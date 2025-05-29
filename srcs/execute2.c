/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 07:03:40 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/29 09:25:19 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

extern int	g_sig;
/*
* Function declaration of helper fuctions
*/
int		execution(t_shell *mini, t_cmd *cmd, int **fd);
pid_t	execute_command(t_shell *mini, t_cmd *cmd, int **fd, int index);
pid_t	*run_commands(t_shell *mini, t_cmd *current, int **fd, int *index);

/*
* Replaces the current process image with a new command using execve().
*
* Parameters:
* - mini: Shell context.
* - cmd: Command to execute.
*
* Returns:
* - Does not return on success; returns 0 only if execve fails (after handling).
*/

int	execution(t_shell *mini, t_cmd *cmd, int **fd)
{
	if (execve(cmd->command, cmd->args, mini->initenv->copy_env) == -1)
	{
		p_exe_error(cmd->args[0], errno, mini->initenv);
		free_env(mini->initenv->copy_env);
		free_fds(fd, mini->num_cmds - 1);
		free(mini->pids);
		cleanup_and_exit(mini);
		exit(mini->initenv->last_status);
	}
	return (0);
}

/*
* Forks a child to execute a command with appropriate pipe redirection.
*
* Sets up pipe redirection, runs pre-execution steps, then execs the command.
*
* Parameters:
* - mini: Shell context.
* - cmd: The command to execute.
* - fd: Array of pipe file descriptors.
* - index: Position of the command in the pipeline.
*
* Returns:
* - PID of the child process, or -1 on fork failure.
*/

pid_t	execute_command(t_shell *mini, t_cmd *cmd, int **fd, int index)
{
	pid_t	pid;
	int		limit;

	limit = mini->num_cmds - 1;
	pid = fork();
	if (pid == -1)
	{
		perror("Fork failed");
		return (-1);
	}
	else if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (index > 0 && (dup2(fd[index - 1][0], STDIN_FILENO) == -1))
			perror_exit("dup2 stdin");
		if (index < limit && (dup2(fd[index][1], STDOUT_FILENO) == -1))
			perror_exit("dup2 stdout");
		close_fds2(fd, limit);
		pre_execute(mini, cmd, fd);
		execution(mini, cmd, fd);
		clear_and_exit(mini);
	}
	return (pid);
}

/*
* Runs all commands in a pipeline by forking and executing them.
*
* Parameters:
* - mini: Shell context.
* - current: First command in the pipeline.
* - fd: Pipe file descriptor array.
* - index: Pointer to an index counter (will be updated).
*
* Returns:
* - Array of PIDs for the child processes, or NULL on error.
*/

pid_t	*run_commands(t_shell *mini, t_cmd *current, int **fd, int *index)
{
	int		i;
	pid_t	pid;
	pid_t	*pids;

	i = *index;
	pids = malloc(sizeof(pid_t) * (mini->num_cmds));
	if (!pids)
		return (NULL);
	mini->pids = pids;
	while (current)
	{
		pid = execute_command(mini, current, fd, i);
		if (pid == -1)
			return (NULL);
		pids[i] = pid;
		current = current->next;
		i++;
	}
	*index = i;
	return (pids);
}
