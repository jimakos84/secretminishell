#include "../includes/shell.h"

static int close_fds(int fd[][2], int limit);
static int init_pipes(int fd[][2], int limit);
static int wait_for_children(int limit);

int execute(t_shell *mini)
{
	int index = 0, limit = mini->num_cmds;
	t_cmd *current = mini->cmds;
	pid_t pid;
	int fd[limit][2];

	init_pipes(fd, limit);
	while(current)
	{
		if((pid = fork()) == -1)
			perror("Fork fialed");
		else if(pid == 0)
		{
			if(current->type == OPRD_CMD)
			{
				int fd;
				if ((fd = open(current->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
					perror("File opening failed!");
					exit (1);
				}
				if(dup2(fd, STDOUT_FILENO) == -1)
				{
					perror("FD duplication failed!");
					exit (1);
				}
				close(fd);
			}
			if(current->type == APRD_CMD)
			{
				int fd;
				if ((fd = open(current->filename, O_WRONLY | O_CREAT | O_APPEND, 0666)) == -1) {
					perror("File opening failed!");
					exit (1);
				}
				if(dup2(fd, STDOUT_FILENO) == -1)
				{
					perror("FD duplication failed!");
					exit (1);
				}
				close(fd);
			}
			if(current->type == IPRD_CMD || current->type == HDRD_CMD)
			{
				int fd;
				if((fd = open(current->filename, O_RDONLY)) == -1)
				{
					perror("File opening failed!");
					exit (1);
				}
				if(dup2(fd, STDIN_FILENO) == -1)
				{
					perror("FD duplication failed!");
					exit (1);
				}
				close(fd);
			}
			if(index > 0)
				dup2(fd[index - 1][0], STDIN_FILENO);
			if(current->next)
				dup2(fd[index][1], STDOUT_FILENO);
			close_fds(fd, limit);
			mini->initenv->copy_env = copy_env(mini->initenv->env);
			if((execve(current->command, current->args, mini->initenv->copy_env)) == -1)
				exit(1);
		}
		current = current->next;
		index++;
	}
	close_fds(fd, limit);
	wait_for_children(limit);
	return (0);
}

static int init_pipes(int fd[][2], int limit)
{
	int i = 0;

	while(i < limit)
	{
		if((pipe(fd[i])) == -1)
		{
			perror("Pipe creation failed");
		}
		i++;
	}
	return (0);
}

static int wait_for_children(int limit)
{
	int i = 0;
	while(i < limit)
	{
		wait(NULL);
		i++;
	}
	return (0);
}

static int close_fds(int fd[][2], int limit)
{
	int i = 0;
	while(i < limit)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
	return (0);
}
