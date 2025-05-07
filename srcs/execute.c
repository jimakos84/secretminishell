/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:24:34 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/04 14:44:50 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

static int	init_pipes(int fd[][2], int limit);
static int	wait_for_children(int count, t_initenv *initenv);
static int	execute_command(t_shell *mini, t_cmd *current,
				int fd[][2], int index);


int	execute(t_shell *mini)
{
	int		index;
	int		limit;
	t_cmd	*current;
	int		fd[1024][2];

	index = 0;
	current = mini->cmds;
	limit = mini->num_cmds - 1;
	if (mini->num_cmds == 1 && current->is_builtin)
	{
		int saved_stdin = dup(STDIN_FILENO);
		int saved_stdout = dup(STDOUT_FILENO);
		if (saved_stdin == -1 || saved_stdout == -1)
			perror_exit("dup failed");

		if (handle_redirections(current) == -1)
		{
			fprintf(stderr, "Redirection failed, skipping command\n");
			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);
			return (1);
		}
		mini->initenv->last_status = check_builtin(mini);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (mini->initenv->last_status);
	}
	if (init_pipes(fd, limit))
		return (1);
	while (current)
	{
		if (execute_command(mini, current, fd, index) == -1)
			return (1);
		current = current->next;
		index++;
	}
	close_fds(fd, limit);
	wait_for_children(index, mini->initenv);
	return (mini->initenv->last_status);
}

static int	init_pipes(int fd[][2], int limit)
{
	int	i;

	i = 0;
	while (i < limit)
	{
		if (pipe(fd[i]) == -1)
		{
			perror("Pipe creation failed");
			return (1);
		}
		i++;
	}
	return (0);
}


static int	wait_for_children(int count, t_initenv *initenv)
{
	int	i = 0;
	int	status;
	int	last_exit_status = 0;

	while (i < count)
	{
		wait(&status);
		if (WIFEXITED(status))
			last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_exit_status = 128 + WTERMSIG(status);
		i++;
	}
	initenv->last_status = last_exit_status;
	return (last_exit_status);
}

static int execute_command(t_shell *mini, t_cmd *current, int fd[][2], int index)
{
	pid_t pid;
	int limit;

	limit = mini->num_cmds - 1;
	pid = fork();
	if (pid == -1)
	{
		perror("Fork failed");
		return -1;
	}
	else if (pid == 0)
	{
		if (index > 0)
		{
			if (dup2(fd[index - 1][0], STDIN_FILENO) == -1)
				perror_exit("dup2 stdin");
		}
		if (index < limit)
		{
			if (dup2(fd[index][1], STDOUT_FILENO) == -1)
				perror_exit("dup2 stdout");
		}
		close_fds(fd, limit);
		if (handle_redirections(current) == -1)
			exit(1);
		if (current->is_builtin)
		{
			if (check_builtin(mini))
				exit(mini->initenv->last_status);
		}
		mini->initenv->copy_env = copy_env(mini->initenv->env);
		if (execve(current->command, current->args, mini->initenv->copy_env) == -1)
		{
			p_exe_error(current->args[0], errno, mini->initenv);
			free_env(mini->initenv->copy_env);
			exit(mini->initenv->last_status);
		}
		free_env(mini->initenv->copy_env);
	}
	return (0);
}


