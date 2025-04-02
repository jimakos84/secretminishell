/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:00:48 by dvlachos          #+#    #+#             */
/*   Updated: 2025/04/02 16:45:50 by dvlachos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	check_builtin(t_shell *mini);
static int	builtin_cd(t_shell *mini);

t_env	*new_node(char *content)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->name = ft_substr(content, 0, (ft_strchr(content, '=') - content));
	if (!node->name)
		return (NULL);
	node->value = ft_strdup(ft_strchr(content, '=') + 1);
	if (!node->value)
		return (NULL);
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
	copy = (char **)malloc((len + 1) * sizeof(char *));
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
	mini->trim = NULL;
	mini->tokens = NULL;
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

static int	quotes_checker(char *input, int len)
{
	int	in_quote;
	int	i;

	i = 0;
	in_quote = 0;
	while (input[i] && i < len)
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			if (in_quote == 0)
				in_quote = input[i];
			else if (in_quote == input[i])
				in_quote = 0;
		}
		i++;
	}
	return (in_quote);
}

static int	_exe_cmd(t_shell *mini)
{
	char	**path_dirs;
	int		i;
	char	*full_path;
	int		status;

	i = 0;
	path_dirs = ft_split(getenv("PATH"), ':');
	while (path_dirs && path_dirs[i])
	{
		full_path = ft_strjoin(path_dirs[i], "/");
		full_path = ft_strjoin(full_path, mini->tokens[0]);
		if (access(full_path, X_OK) == 0)
		{
			if (fork() == 0)
				execve(full_path, mini->tokens, mini->copy_env);
			wait(&status);
			return (0);
		}
		free(full_path);
		i++;
	}
	if (check_builtin(mini))
		printf("%s: command not found\n", mini->tokens[0]);
	return (0);
}

static int	tokenize(t_shell *mini)
{
	char	buffer[1024];
	int		buf_i;
	int		token_i;
	int		i;
	char	quote;

	buf_i = 0;
	token_i = 0;
	i = 0;
	mini->tokens = malloc(1024 * sizeof(char *));
	if (!mini->tokens)
		return (1);
	if (quotes_checker(mini->trim, ft_strlen(mini->trim)))
	{
		ft_putstr_fd("Error: Unmatched quotes\n", 2);
		free(mini->tokens);
		return (1);
	}
	while (mini->trim[i])
	{
		if (mini->trim[i] == '\'' || mini->trim[i] == '"')
		{
			quote = mini->trim[i];
			i++;
			while (mini->trim[i] && mini->trim[i] != quote)
			{
				if (mini->trim[i] == '\\' && quote == '"')
				{
					i++;
					if (mini->trim[i])
						buffer[buf_i++] = mini->trim[i];
				}
				else
					buffer[buf_i++] = mini->trim[i];
				i++;
			}
			i++;
		}
		else if (ft_isspace(mini->trim[i]))
		{
			if (buf_i > 0)
			{
				buffer[buf_i] = '\0';
				mini->tokens[token_i++] = ft_strdup(buffer);
				buf_i = 0;
			}
			i++;
		}
		else if (mini->trim[i] == '\\')
		{
			i++;
			if (mini->trim[i])
				buffer[buf_i++] = mini->trim[i];
			i++;
		}
		else
		{
			buffer[buf_i++] = mini->trim[i];
			i++;
		}
	}
	if (buf_i > 0)
	{
		buffer[buf_i] = '\0';
		mini->tokens[token_i++] = ft_strdup(buffer);
	}
	mini->tokens[token_i] = NULL;
	return (0);
}

static char	*extract_env_value(t_shell *mini, char *name)
{
	while (mini->env)
	{
		if (ft_strncmp(name, mini->env->name, sizeof(name)) == 0)
			return (mini->env->value);
		mini->env = mini->env->next;
	}
	return (NULL);
}

static int	builtin_cd(t_shell *mini)
{
	char	*home;
	char	*dir;

	home = extract_env_value(mini, "HOME");
	dir = NULL;
	if (!home)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		return (1);
	}
	if (!mini->tokens[1])
		chdir(home);
	else
	{	
		if ((ft_strncmp("~", mini->tokens[1], 1) == 0))
		{
			if (mini->tokens[1] && !mini->tokens[1][1])
				chdir(home);
			else
			{
				dir = ft_strjoin(home, ft_strchr(mini->tokens[1], '~') + 1);
			}
		}
		else
		{
			dir = ft_strjoin(getcwd(NULL, 0), "/");
			dir = ft_strjoin(dir, mini->tokens[1]);
		}
		if (chdir(dir))
		{
			printf("cd: %s: No such file or directory\n", mini->tokens[1]);
			return (1);
		}
	}
	return (0);
}

static int	check_builtin(t_shell *mini)
{
	if (ft_strncmp("cd", mini->tokens[0], 2) == 0)
	{
		if (builtin_cd(mini))
			return (1);
		else
			return (0);
	}
	else
		return (1);
}

static int	input_handler(t_shell *mini, char *input)
{
	(void)mini;
	add_history(input);
	if (mini->trim)
		free(mini->trim);
	mini->trim = ft_strtrim(input, " \t\n\r\v");
	if (!mini->trim)
		return (1);
	if (tokenize(mini))
		return (1);
	_exe_cmd(mini);
	return (0);
}

static void	_free_env(t_shell *mini)
{
	t_env	*env;
	t_env	*next;
	int		i;

	i = 0;
	while (mini->copy_env && mini->copy_env[i])
		free(mini->copy_env[i++]);
	if (mini->copy_env)
		free(mini->copy_env);
	env = mini->env;
	while (env != NULL)
	{
		next = env->next;
		if (env->name)
			free(env->name);
		if (env->value)
			free(env->value);
		free(env);
		env = next;
	}
	mini->env = NULL;
	if (mini->trim)
		free(mini->trim);
	while (mini->tokens && *mini->tokens)
		free(*mini->tokens++);
	if (mini->tokens)
		free(*mini->tokens);
	free(mini);
}

static int	_prompt(t_shell *mini, int status)
{
	char	*input;

	(void)mini;
	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL || !ft_strncmp("exit", input, 4))
		{
			free(input);
			_free_env(mini);
			break ;
		}
		if (ft_isempty(input))
		{
			free(input);
			continue ;
		}
		if (input && *input)
		{
			input_handler(mini, input);
			free(input);
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

void	_handler(int signo)
{
	(void)signo;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
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
	signal(SIGINT, _handler);
	signal(SIGQUIT, SIG_IGN);
	start_shell(status, envp);
	return (EXIT_SUCCESS);
}
