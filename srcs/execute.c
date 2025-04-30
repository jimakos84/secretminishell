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
int	handle_redirections(t_cmd *current);
//static int	handle_dup2(int old_fd, int new_fd);
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

int handle_redirections(t_cmd *cmd)
{
    t_redir *r = cmd->redir_list;
    int fd;

    while (r)
    {
        if (!r->filename)
        {
            fprintf(stderr, "Redirection error: missing filename\n");
            return -1;
        }

        if (r->type == OPRD_CMD)  // >
        {
            fd = open(r->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fd < 0 || dup2(fd, STDOUT_FILENO) < 0)
            {
                perror("Failed to redirect stdout");
                if (fd >= 0) close(fd);
                return -1;
            }
        }
        else if (r->type == APRD_CMD)  // >>
        {
            fd = open(r->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (fd < 0 || dup2(fd, STDOUT_FILENO) < 0)
            {
                perror("Failed to redirect stdout (append)");
                if (fd >= 0) close(fd);
                return -1;
            }
        }
        else if (r->type == IPRD_CMD)  // <
        {
            fd = open(r->filename, O_RDONLY);
            if (fd < 0 || dup2(fd, STDIN_FILENO) < 0)
            {
                perror("Failed to redirect stdin");
                if (fd >= 0) close(fd);
                return -1;
            }
        }
        else
        {
            fprintf(stderr, "Unknown redirection type\n");
            return -1;
        }

        if (fd >= 0)
            close(fd);

        r = r->next;
    }

    return 0;
}



// static int	handle_dup2(int old_fd, int new_fd)
// {
// 	if (dup2(old_fd, new_fd) == -1)
// 	{
// 		perror("dup2 failed");
// 		return (-1);
// 	}
// 	return (0);
// }

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

