#include "../includes/shell.h"

int syntax_error(char *input, char *msg, int code)
{
	free(input);
	ft_putendl_fd(msg, 2);
	return (code);
}

void	p_exe_error(char *command, int err)
{
	if (err == ENOENT)
	{
		ft_putstr_fd(command, 2);
		ft_putendl_fd(": command not found", 2);
	}
	else if (err == EACCES)
	{
		ft_putstr_fd(command, 2);
		ft_putendl_fd(": Permission denied", 2);
	}
}
