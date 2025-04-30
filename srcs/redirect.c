#include "../includes/shell.h"

int	close_fds(int fd[][2], int limit)
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
			printf("WE GET TO heREeeee!\n");
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
			printf("WE GET TO heRE!\n");
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