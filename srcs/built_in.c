/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:18:12 by dvlachos          #+#    #+#             */
/*   Updated: 2025/04/29 10:14:13 by dvlachos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

static void	updatewd(t_shell *mini, char *newpwd, char *oldpwd)
{
	t_env	*env;

	env = mini->initenv->env;
	while (env)
	{
		if (ft_strncmp("PWD", env->name, 3) == 0)
		{
			free(env->value);
			env->value = ft_strdup(newpwd);
		}
		if (ft_strncmp("OLDPWD", env->name, 6) == 0)
		{
			free(env->value);
			env->value = ft_strdup(oldpwd);
		}
		env = env->next;
	}
}

static int	print_cd_error(char *path, char *oldpwd)
{
	printf("cd: %s: No such file or directory\n", path);
	free(oldpwd);
	return (1);
}

static int	try_change_dir(t_shell *mini, char *target, char *oldpwd)
{
	if (chdir(target) != 0)
		return (print_cd_error(target, oldpwd));
	updatewd(mini, getcwd(NULL, 0), oldpwd);
	return (0);
}

int	builtin_cd(t_shell *mini)
{
	char	*oldpwd;
	char	*home;

	home = mini->initenv->home;
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (1);
	if (!mini->cmds->args[1])
	{
		if (!home || !home[0])
		{
			printf("cd: HOME not set\n");
			free(oldpwd);
			return (1);
		}
		if (chdir(home) != 0)
			return (print_cd_error(home, oldpwd));
		updatewd(mini, getcwd(NULL, 0), oldpwd);
		return (0);
	}
	return (try_change_dir(mini, mini->cmds->args[1], oldpwd));
}

// will work with the same way env command on bash

int	builtin_env(t_shell *mini)
{
	t_env	*temp;

	temp = mini->initenv->env;
	if (!mini->cmds->args[1])
	{
		while (temp)
		{
			printf("%s", temp->name);
			printf("=");
			printf("%s\n", temp->value);
			temp = temp->next;
		}
		return (0);
	}
	else
		return (1);
}

int	builtin_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	return (2);
}

//to check if the command is in the built_in commands 
//list, if yes then use the custome one

int	check_builtin(t_shell *mini)
{
	char	*cmd;

	cmd = mini->cmds->cmd;
	if (mini->cmds->cmd)
	{
		if (ft_strncmp("cd", cmd, ft_strlen(cmd)) == 0)
		{
			if (builtin_cd(mini))
				return (1);
			else
				return (2);
		}
		if (ft_strncmp("env", cmd, ft_strlen(cmd)) == 0)
		{
			if (builtin_env(mini))
				return (1);
			else
				return (2);
		}
		if (ft_strncmp("pwd", cmd, ft_strlen(cmd)) == 0)
			printf("%s\n", getcwd(NULL, 0));
		if (ft_strncmp("exit", cmd, ft_strlen(cmd)) == 0)
		{
			printf("exit\n");
			clear_env(mini->initenv->env);
			free(mini->initenv->home);
			free(mini->initenv);
			clear_and_exit(mini);
			rl_clear_history();
			exit (1);
		}
		if (ft_strncmp("unset", cmd, ft_strlen(cmd)) == 0)
		{
			builtin_unset(mini);
			return (0);
		}
		else
			return (0);
	}
	return (0);
}
