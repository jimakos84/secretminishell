/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 09:44:50 by dvlachos          #+#    #+#             */
/*   Updated: 2025/04/21 09:44:52 by dvlachos         ###   ########.fr       */
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
	char	*home;
	char	*dir;
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	home = extract_env_value(mini->initenv, "HOME");
	dir = NULL;
	if (!home)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		return (1);
	}
	if (!mini->cmds->args[1])
		chdir(home);
	else
	{	
		if ((ft_strncmp("~", mini->cmds->args[1], 1) == 0))
		{
			if (mini->cmds->args[1] && !mini->cmds->args[1][1])
				dir = home;
			else
				dir = ft_strjoin(home, ft_strchr(mini->cmds->args[1], '~') + 1);
		}
		else
		{
			dir = ft_strjoin(getcwd(NULL, 0), "/");
			dir = ft_strjoin(dir, mini->cmds->args[1]);
		}
		if (chdir(dir))
		{
			printf("cd: %s: No such file or directory\n", mini->cmds->args[1]);
			return (1);
		}
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
/**
 * to check if the command is in the built_in commands list, if yes then use the custome one
*/

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
			builtin_unset(mini, mini->cmds->args);
			return (0);
		}
	    else
		    return (0);
    }
	return (0);
}
