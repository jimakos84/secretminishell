/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:18:12 by dvlachos          #+#    #+#             */
/*   Updated: 2025/04/23 14:21:18 by dvlachos         ###   ########.fr       */
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

int	builtin_cd(t_shell *mini)
{
	char	*dir;
	char	*oldpwd;
	char	*home;

	home = mini->initenv->home;
	oldpwd = getcwd(NULL, 0);
	if (!mini->cmds->args[1])
	{
		if (!home[0])
		{
			printf("cd: HOME not set\n");
			return (1);
		}
		chdir(home);
		updatewd(mini, home, oldpwd);
		return (0);
	}
	dir = NULL;
	dir = ft_strjoin(getcwd(NULL, 0), "/");
	dir = ft_strjoin(dir, mini->cmds->args[1]);
	if (chdir(dir))
	{
		printf("cd: %s: No such file or directory\n", mini->cmds->args[1]);
		return (1);
	}
	updatewd(mini, getcwd(NULL, 0), oldpwd);
	return (0);
}

/**
 * will work with the same way env command on bash
*/
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
		if (ft_strncmp("cd", cmd, sizeof(cmd)) == 0)
		{
			if (builtin_cd(mini))
				return (1);
			else
				return (2);
		}
		if (ft_strncmp("env", cmd, sizeof(cmd)) == 0)
		{
			if (builtin_env(mini))
				return (1);
			else
				return (2);
		}
		if (ft_strncmp("pwd", cmd, sizeof(cmd)) == 0)
			printf("%s\n", getcwd(NULL, 0));
		if (ft_strncmp("exit", cmd, sizeof(cmd)) == 0)
		{
			printf("exit\n");
			clear_and_exit(mini);
			exit (mini->status);
		}
		if (ft_strncmp("unset", cmd, sizeof(cmd)) == 0)
		{
			builtin_unset(mini);
			return (0);
		}
		else
			return (0);
	}
	return (0);
}
