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
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putendl_fd(": No such file or directory", 2);
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

	home = extract_env_value(mini->initenv, "HOME");
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (1);
	if (mini->cmds->args[2])
	{
		ft_putendl_fd("too many arguments", 2);
		return (1);
	}
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
		}
		if (ft_strncmp("env", cmd, ft_strlen(cmd)) == 0)
		{
			if (builtin_env(mini))
				return (1);
		}
		if (ft_strncmp("pwd", cmd, ft_strlen(cmd)) == 0)
				printf("%s\n", getcwd(NULL, 0));
		if (ft_strncmp("exit", cmd, ft_strlen(cmd)) == 0)
			exit_mini(mini);
		if (ft_strncmp("unset", cmd, ft_strlen(cmd)) == 0)
		{
			builtin_unset(mini);
			return (0);
		}
		if (ft_strncmp("export", cmd, ft_strlen(cmd)) == 0)
		{
			if (builtin_export(mini))
				return (1);
		}
		if (ft_strncmp("echo", cmd, ft_strlen(cmd)) == 0)
		{
			if (builtin_echo(mini))
				return (1);
		}
		else
			return (0);
	}
	return (0);
}
