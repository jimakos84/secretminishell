/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:24:34 by dvlachos          #+#    #+#             */
/*   Updated: 2025/04/29 10:31:15 by dvlachos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

static int	init_pipes(int fd[][2], int limit);
static int	wait_for_children(int count);
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
		check_builtin(mini);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (0);
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
	wait_for_children(index);
	return (0);
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


static int	wait_for_children(int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		wait(NULL);
		i++;
	}
	return (0);
}

static int execute_command(t_shell *mini, t_cmd *current,
	int fd[][2], int index)
{
pid_t pid;

(void)index;
pid = fork();
if (pid == -1)
{
perror("Fork failed");
return (-1);
}
else if (pid == 0)
{
	close_fds(fd, mini->num_cmds - 1);
	handle_redirections(current);
	mini->initenv->copy_env = copy_env(mini->initenv->env);
	if (current->is_builtin)
	{
		check_builtin(mini);
		free_env(mini->initenv->copy_env);
		exit(0);
	}
	if (execve(current->command, current->args, mini->initenv->copy_env) == -1)
	{
		if (errno == ENOENT || errno == EACCES)
			p_exe_error(current->args[0], errno);
		else
			perror(current->args[0]);
		free_env(mini->initenv->copy_env);
		mini->initenv->copy_env = NULL;
		exit(127);
	}
}
return (0);
}

