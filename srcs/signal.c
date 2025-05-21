/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:03:08 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/18 15:31:02 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

extern int	g_sig;

/*
 * Function declaration of helper fuctions
*/
void	init_sig(void);
void	handler(int sig);

/*
* Signal handler function for SIGINT (Ctrl+C).
* - Clears the current input line in the readline buffer.
* - Moves to a new line and redisplays the prompt.
*
* Parameters:
* - sig: The signal number received.
*/

void	handler(int sig)
{
	g_sig = sig;
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

/*
* Initializes signal handling for the shell.
* Sets a custom handler for SIGINT (interrupt signal).
*/

void	handler_heredoc(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	g_sig = sig;
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		write(1, "\n", 1);
		close(STDIN_FILENO);
	}
}

void	init_sig(void)
{
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
}

void	init_sig_heredoc(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
}
