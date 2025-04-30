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
static int	close_fds(int fd[][2], int limit);
static int	wait_for_children(int count);
static void	handle_redirections(t_cmd *current);
static int	handle_dup2(int old_fd, int new_fd);
//static void	handle_file_redirection(t_cmd *current, int fd_type);
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
	
		handle_redirections(current);
		check_builtin(mini);
	
		// Restore original stdin and stdout
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

static int	close_fds(int fd[][2], int limit)
{
	int	i;

	i = 0;
	while (i < limit)
	{
		close(fd[i][0]);
		close(fd[i][1]);
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

static void handle_redirections(t_cmd *cmd)
{
    t_redir *r = cmd->redir_list;
    int fd;

    while (r)
    {
        if (r->type == OPRD_CMD)  // >
        {
            printf("Redirecting stdout to %s\n", r->filename);
            fd = open(r->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        }
        else if (r->type == APRD_CMD)  // >>
        {
            printf("Redirecting stdout to %s with append\n", r->filename);
            fd = open(r->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
        }
        else if (r->type == IPRD_CMD)  // <
        {
            printf("Redirecting stdin from %s\n", r->filename);
            fd = open(r->filename, O_RDONLY);
        }
        else
        {
            r = r->next;
            continue;
        }

        if (fd == -1)
        {
            perror("Redirection failed");
            exit(1);
        }

        if ((r->type == IPRD_CMD) && dup2(fd, STDIN_FILENO) == -1)
        {
            perror("dup2 failed for stdin");
            close(fd);
            exit(1);
        }
        else if ((r->type == OPRD_CMD || r->type == APRD_CMD) && dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("dup2 failed for stdout");
            close(fd);
            exit(1);
        }

        close(fd);
        r = r->next;
    }
}


static int	handle_dup2(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		perror("dup2 failed");
		return (-1);
	}
	return (0);
}

// static void	handle_file_redirection(t_cmd *current, int fd_type)
// {
// 	int	fd;

// 	if (fd_type == O_WRONLY)
// 	{
// 		if (current->type == OPRD_CMD)
// 			fd = open(current->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
// 		else
// 			fd = open(current->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
// 	}
// 	else
// 		fd = open(current->filename, O_RDONLY);
// 	if (fd == -1)
// 	{
// 		perror("File opening failed");
// 		exit(1);
// 	}
// 	if (dup2(fd, fd_type == O_WRONLY ? STDOUT_FILENO : STDIN_FILENO) == -1)
// 	{
// 		perror("dup2 failed");
// 		exit(1);
// 	}
// 	close(fd);
// }

static int execute_command(t_shell *mini, t_cmd *current,
	int fd[][2], int index)
{
pid_t pid;

pid = fork();
if (pid == -1)
{
perror("Fork failed");
return (-1);
}
else if (pid == 0)
{
// Set up pipes for previous commands
if (index > 0 && handle_dup2(fd[index - 1][0], STDIN_FILENO) == -1)
	exit(1);
if (current->next && handle_dup2(fd[index][1], STDOUT_FILENO) == -1)
	exit(1);

// Close all pipe file descriptors in the child process
close_fds(fd, mini->num_cmds - 1);

// Handle redirections for stdin/stdout
handle_redirections(current);

// Debugging: check the file descriptors
printf("stdin: %d, stdout: %d\n", dup(STDIN_FILENO), dup(STDOUT_FILENO));

mini->initenv->copy_env = copy_env(mini->initenv->env);

// If it's a builtin command, handle it
if (current->is_builtin)
{
	check_builtin(mini);
	free_env(mini->initenv->copy_env);
	exit(0);
}

// Debugging: print the command and arguments
printf("Executing command: %s\n", current->command);
for (int i = 0; current->args[i]; i++)
{
	printf("Arg %d: %s\n", i, current->args[i]);
}

// Execute the command
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

