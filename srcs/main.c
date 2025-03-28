/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:00:48 by dvlachos          #+#    #+#             */
/*   Updated: 2025/03/28 14:42:29 by dvlachos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_env	*new_node(char *content)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->name = ft_substr(content, 0, (ft_strchr(content, '=') - content));
	node->value = ft_strdup(ft_strchr(content, '=') + 1);
	node->next = NULL;
	return (node);
}

void	add_to_list(t_env **env, char *content)
{
	t_env	*new;
	t_env	*current;

	new = new_node(content);
	if (!new)
		return ;
	if (*env == NULL)
		*env = new;
	else
	{
		current = *env;
		while (current->next != NULL)
			current = current->next;
		current->next = new;
	}
}

void	list_env(t_env **env, char **envp)
{
	while (envp && *envp)
		add_to_list(env, *envp++);
}

static int	ft_arraylen(char **envp)
{
	int		len;

	len = 0;
	while (envp && envp[len])
		len++;
	return (len);
}

static char	**copy_env(char **envp)
{
	int		len;
	int		i;
	char	**copy;

	len = ft_arraylen(envp);
	copy = (char **)malloc(len * sizeof(char *) + 1);
	if (!copy)
		return (NULL);
	i = 0;
	while (envp && envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			while (i > 0)
				free(copy[--i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

static int	init_shell(t_shell *mini, char **envp)
{
	mini->env = NULL;
	list_env(&mini->env, envp);
	mini->copy_env = NULL;
	mini->copy_env = copy_env(envp);
	return (0);
}

int	ft_isempty(char *str)
{
	while (*str)
	{
		if (!ft_isspace(*str))
			return (0);
		str++;
	}
	return (1);
}

static int	handle_input(t_shell *mini, char *input)
{
	char	**token;
	char	*path;
	char	*_path;
	int		status;

	(void)mini;
	path = "/bin/";
	add_history(input);
	if (!ft_strncmp(input, "exit", 4))
		exit (1);
	token = ft_split(input, ' ');
	_path = ft_strjoin(path, token[0]);
	if (fork() == 0)
		execve(_path, token, mini->copy_env);
	wait(&status);
	return (0);
}

static int	_prompt(t_shell *mini, int status)
{
	char	*input;

	(void)mini;
	while (1)
	{
		input = readline("tell me all your secrets> ");
		if (input == NULL)
			break ;
		if (ft_isempty(input))
		{
			free(input);
			continue ;
		}
		if (input && *input)
		{
			handle_input(mini, input);
			continue ;
		}
	}
	return (status);
}

static int	start_shell(int status, char **envp)
{
	t_shell	*mini;

	mini = malloc(sizeof(t_shell));
	if (!mini)
	{
		ft_putstr_fd("mini struct malloc failed\n", 2);
		status = 1;
		return (status);
	}
	if (init_shell(mini, envp))
	{
		status = 1;
		return (status);
	}
	status = _prompt(mini, status);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	int		status;

	status = 0;
	(void)av;
	if (ac != 1)
	{
		printf("Run just ./minishell, no arguments allowed!\n");
		return (1);
	}
	start_shell(status, envp);
	return (EXIT_SUCCESS);
}
