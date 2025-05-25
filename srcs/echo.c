/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:37:14 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/25 14:54:57 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Function declaration of helper fuctions
*/
void	print_args(char **args, int *index);
int		proper_flag(char *str);

/*
* Implements the `echo` built-in command.
*
* Behavior:
* - Supports the `-n` flag to suppress the trailing newline.
* - Prints arguments separated by spaces.
*
* Parameters:
* - cmd: Pointer to the command structure containing arguments.
*
* Returns:
* - Always returns 0.
*/

int	builtin_echo(t_cmd *cmd)
{
	char	**args;
	int		i;
	int		set_n;

	i = 1;
	set_n = 0;
	args = cmd->args;
	if (cmd->num_args == 0)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	else
	{
		while (args[i] && proper_flag(args[i]))
		{
			set_n = 1;
			i++;
		}
		print_args(args, &i);
		if (!set_n)
			write(STDOUT_FILENO, "\n", 1);
	}
	return (0);
}

/*
* Prints command arguments starting from the given index, separated by spaces.
*
* Parameters:
* - args: NULL-terminated array of argument strings.
* - index: Pointer to the starting index
* (typically 1 for skipping the command name).
*/

void	print_args(char **args, int *index)
{
	int	i;

	i = *index;
	while (args && args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		i++;
		if (args[i])
			write(1, " ", 1);
	}
}

int	proper_flag(char *str)
{
	int	i;

	i = 0;
	if (str && str[i] == '-')
	{
		i++;
		while (str && str[i] == 'n')
			i++;
		if (str[i] == '\0')
			return (1);
		return (0);
	}
	return (0);
}
