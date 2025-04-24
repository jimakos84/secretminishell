#include "../includes/shell.h"

void handler(int sig)
{
	if(sig == SIGINT)
	{
		rl_replace_line("", 0);
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
	}
}

void init_sig(void)
{
	signal(SIGINT, handler);
}
