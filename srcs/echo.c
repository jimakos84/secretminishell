/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:37:14 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/09 06:50:31 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Function declaration of helper fuctions
*/
void	print_args(char **args, int *index);

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
	if (!args[i])
	{
		write(1, "\n", 1);
		return (0);
	}
	else
	{
		while (ft_strncmp(args[i], "-n", 2) == 0)
		{
			set_n = 1;
			i++;
		}
		print_args(args, &i);
		if (!set_n)
			write(1, "\n", 1);
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
