#include "../includes/shell.h"

int syntax_error(char *input, char *msg)
{
	free(input);
	ft_putendl_fd(msg, 2);
	return (1);
}
