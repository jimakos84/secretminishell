/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:03:08 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/07 13:09:25 by tsomacha         ###   ########.fr       */
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
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
	}
}

/*
* Initializes signal handling for the shell.
* Sets a custom handler for SIGINT (interrupt signal).
*/

void	init_sig(void)
{	
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
}
