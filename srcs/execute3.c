/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 07:03:51 by tsomacha          #+#    #+#             */
/*   Updated: 2025/06/01 13:31:20 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Function declaration of helper fuctions
*/
void	free_fds(int **fd, int count);
void	close_fds2(int **fd, int limit);
void	pre_execute(t_shell *mini, t_cmd *cmd, int **fd);

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
		if (close(fd[i][0]) == -1)
			perror("close fd[0] fails");
		if (close(fd[i][1]) == -1)
			perror("close fd[1] fails");
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

void	pre_execute(t_shell *mini, t_cmd *cmd, int **fd)
{
	if (handle_redirections(cmd) == -1)
	{
		free_fds(fd, mini->num_cmds - 1);
		free(mini->pids);
		clear_env(mini->initenv->env);
		free(mini->initenv);
		clear_and_exit(mini);
		exit(1);
	}
	if (cmd->is_builtin)
		exit_builtin(cmd, mini, fd);
	check_stat(cmd->command, mini, fd);
	mini->initenv->copy_env = copy_env(mini->initenv->env);
	if (!cmd->command)
	{
		free_fds(fd, mini->num_cmds - 1);
		free(mini->pids);
		clear_array(mini->initenv->copy_env);
		clear_env(mini->initenv->env);
		free(mini->initenv);
		clear_and_exit(mini);
		exit (0);
	}
}
