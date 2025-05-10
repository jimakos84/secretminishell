/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 07:03:51 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/10 07:08:57 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Function declaration of helper fuctions
*/
void	free_fds(int **fd, int count);
void	close_fds2(int **fd, int limit);
void	pre_execute(t_shell *mini, t_cmd *cmd);

/*
* Frees a dynamically allocated array of pipe file descriptors.
*
* Parameters:
* - fd: Array of pipe fd pairs.
* - count: Number of fd pairs.
*/

void	free_fds(int **fd, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(fd[i]);
		i++;
	}
	free(fd);
}

/*
* Closes all pipe file descriptors up to a given limit.
*
* Parameters:
* - fd: Array of pipe fd pairs.
* - limit: Number of pipe pairs to close.
*/

void	close_fds2(int **fd, int limit)
{
	int	i;

	i = 0;
	while (i < limit)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
}

/*
* Prepares a command for execution in a child process.
*
* Handles redirections, built-ins, and command path checks.
*
* Parameters:
* - mini: Shell context.
* - cmd: The command to prepare.
*/

void	pre_execute(t_shell *mini, t_cmd *cmd)
{
	if (handle_redirections(cmd) == -1)
		exit(1);
	if (cmd->is_builtin)
		exit(check_builtin(cmd, mini));
	check_stat(cmd->command);
	mini->initenv->copy_env = copy_env(mini->initenv->env);
	if (!cmd->command)
		exit (1);
}
