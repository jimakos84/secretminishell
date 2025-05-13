/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 04:35:12 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/09 06:46:33 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Function declaration of helper fuctions
*/
void	clear_env(t_env *env);
void	free_redirections(t_redir *redir_list);
int		cleanup_and_exit(t_shell *mini);

/*
* Frees a linked list of environment variables.
*
* Parameters:
* - env: Pointer to the head of the environment list.
*/

void	clear_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->name);
		free(env->value);
		free(env);
		env = tmp;
	}
}

/*
* Frees a linked list of redirection structures and their filenames.
*
* Parameters:
* - redir_list: Pointer to the head of the redirection list.
*/

void	free_redirections(t_redir *redir_list)
{
	t_redir	*tmp;

	while (redir_list)
	{
		tmp = redir_list->next;
		if (redir_list->filename)
			free(redir_list->filename);
		free(redir_list);
		redir_list = tmp;
	}
}

/*
* Fully cleans up shell resources and exits with the last recorded exit status.
*
* Parameters:
* - mini: Pointer to the shell context.
*
* Returns:
* - The exit status (although this is not reached after `exit()` is called).
*/

int	cleanup_and_exit(t_shell *mini)
{
	int	status;

	status = mini->initenv->last_status;
	clear_env(mini->initenv->env);
	free(mini->initenv);
	if (mini->_stdin != -1)
		close(mini->_stdin);
	if (mini->_stdout != -1)
		close(mini->_stdout);
	clear_and_exit(mini);
	rl_clear_history();
	exit(status);
	return (status);
}
