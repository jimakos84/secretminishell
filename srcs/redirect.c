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
    t_redir *r;
    int fd;
    char    *pmpt;

    pmpt = NULL;
    if (!cmd || !cmd->redir_list)
        return (0);  // No redirections to handle
    r = cmd->redir_list;
    while (r)
    {
        if (!r->filename)
        {
            fprintf(stderr, "Redirection error: missing filename\n");
            return -1;
        }
        // if (!builtin_cmd(cmd->cmd) && access(cmd->cmd, F_OK) != 0)  // Check only non-builtins with access()
        // {
        //     fprintf(stderr, "%s: command not found\n", cmd->cmd);
        //     return -1;  // Command not found, don't proceed with redirection
        // }
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
        else if (r->type == HDRD_CMD)
        {
            fd = open(CACHE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            while((pmpt = readline(">")) != NULL)
            {
                if(ft_strncmp(r->filename, pmpt, ft_strlen(pmpt)) == 0)
                {
                    free(pmpt);
                    break;
                }
                write(fd, pmpt, ft_strlen(pmpt));
                write(fd, "\n", 2);
                free(pmpt);
            }
            close(fd);
            fd = open(CACHE, O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
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