/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:18:12 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/21 16:07:16 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Function declaration of helper fuctions
*/
void	updatewd(t_shell *mini, char *newpwd, char *oldpwd);
int		print_cd_error(char *path, char *oldpwd);
int		try_change_dir(t_shell *mini, char *target, char *oldpwd);
int		builtin_cd(t_shell *mini);
int		check_builtin(t_cmd *current, t_shell *mini);

/*
* Updates the PWD and OLDPWD environment variables in the shell.
*
* Parameters:
* - mini: Pointer to the shell context.
* - newpwd: The new working directory path.
* - oldpwd: The previous working directory path.
*/

void	updatewd(t_shell *mini, char *newpwd, char *oldpwd)
{
	t_env	*env;

	env = mini->initenv->env;
	while (env)
	{
		if (ft_strncmp("PWD", env->name, 3) == 0 && newpwd)
		{
			free(env->value);
			env->value = ft_strdup(newpwd);
		}
		if (ft_strncmp("OLDPWD", env->name, 6) == 0 && oldpwd)
		{
			free(env->value);
			env->value = ft_strdup(oldpwd);
		}
		env = env->next;
	}
}

/*
* Prints an error message when `cd` fails to change directory, then frees oldpwd.
*
* Parameters:
* - path: The path that caused the error.
* - oldpwd: The previous working directory (to be freed).
*
* Returns:
* - Always returns 1 (error code).
*/

int	print_cd_error(char *path, char *oldpwd)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putendl_fd(": No such file or directory", 2);
	free(oldpwd);
	return (1);
}

/*
* Attempts to change the working directory to the given target.
*
* If successful, updates PWD and OLDPWD. If not, prints an error message.
*
* Parameters:
* - mini: Pointer to the shell context.
* - target: Target directory to switch to.
* - oldpwd: The old working directory (to be freed after update).
*
* Returns:
* - 0 on success, 1 on failure.
*/

int	try_change_dir(t_shell *mini, char *target, char *oldpwd)
{
	char	*newpwd;

	if (chdir(target) != 0)
		return (print_cd_error(target, oldpwd));
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		perror("getcwd");
	updatewd(mini, newpwd, oldpwd);
	free(newpwd);
	free(oldpwd);
	return (0);
}

/*
* Implements the `cd` built-in command.
*
* Behavior:
* - No arguments: attempts to go to HOME.
* - One argument: changes to the specified directory.
* - More than one argument: prints an error.
*
* Parameters:
* - mini: Pointer to the shell context.
*
* Returns:
* - 0 on success, 1 on failure.
*/

int	builtin_cd(t_shell *mini)
{
	char	*oldpwd;
	char	*home;
	char	*newpwd;

	home = extract_env_value(mini->initenv, "HOME");
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		oldpwd = ft_strdup(extract_env_value(mini->initenv, "PWD"));
	if (mini->cmds->args[1] && mini->cmds->args[2])
		return (echo_error(" too many arguments", oldpwd));
	if (!mini->cmds->args[1])
	{
		if (!home || !home[0])
			return (home_not_set(oldpwd));
		if (chdir(home) != 0)
			return (print_cd_error(home, oldpwd));
		newpwd = getcwd(NULL, 0);
		if (!newpwd)
			return (1);
		updatewd(mini, newpwd, oldpwd);
		free(oldpwd);
		free(newpwd);
		return (0);
	}
	return (try_change_dir(mini, mini->cmds->args[1], oldpwd));
}

/*
* Executes a built-in shell command if matched.
*
* Supported built-ins: cd, env, pwd, exit, unset, export, echo.
*
* Parameters:
* - current: The command node containing the user input.
* - mini: Pointer to the shell context.
*
* Returns:
* - The result of the built-in command, or 0 if the command is not built-in.
*/

int	check_builtin(t_cmd *current, t_shell *mini)
{
	char	*cmd;

	cmd = current->cmd;
	if (cmd)
	{
		if (ft_strncmp("cd", cmd, 3) == 0)
			return (builtin_cd(mini));
		if (ft_strncmp("env", cmd, 4) == 0)
			return (builtin_env(mini));
		if (ft_strncmp("pwd", cmd, 4) == 0)
			return (builtin_pwd(mini->initenv));
		if (ft_strncmp("exit", cmd, 5) == 0)
			return (exit_mini(mini));
		if (ft_strncmp("unset", cmd, 6) == 0)
			return (builtin_unset(mini));
		if (ft_strncmp("export", cmd, 7) == 0)
			return (builtin_export(mini));
		if (ft_strcmp("echo", cmd) == 0)
			return (builtin_echo(current));
		else
			return (0);
	}
	return (0);
}
