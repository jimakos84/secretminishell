/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 09:30:51 by dvlachos          #+#    #+#             */
/*   Updated: 2025/04/21 09:30:54 by dvlachos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/shell.h"

volatile sig_atomic_t	g_sig = 0;

void	init_env(t_initenv **initenv, char **envp)
{
	*initenv = malloc(sizeof(t_initenv));
	(*initenv)->copy_env = NULL;
	(*initenv)->env = NULL;
	(*initenv)->home = NULL;
	(*initenv)->last_status = 0;
	list_env(&(*initenv)->env, envp);
	(*initenv)->home = ft_strdup(extract_env_value(*initenv, "HOME"));
}

static void	init_shell(int status, t_initenv *initenv)
{
	char		*input;

	(void)status;
	input = readline("minishell> ");
	if (!input)
		exit (1);
	if (ft_isempty(input))
	{
		free(input);
		return ;
	}
	add_history(input);
	status = activate_shell(input, initenv);
	free(input);
}

int	main(int ac, char **av, char **envp)
{
	t_initenv	*initenv;

	initenv = NULL;
	(void)av;
	if (ac == 1)
	{
		init_sig();
		init_env(&initenv, envp);
		while (1)
			init_shell(initenv->last_status, initenv);
	}
	return (initenv->last_status);
}
