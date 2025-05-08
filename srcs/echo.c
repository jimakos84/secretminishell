/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:37:14 by dvlachos          #+#    #+#             */
/*   Updated: 2025/04/29 17:37:16 by dvlachos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int    builtin_echo(t_cmd *cmd)
{
    char    **args;
    int     i;

    i = 1;
    args = cmd->args;
    if (!args[i])
    {
        printf("\n");
        return (0);
    }
    if (ft_strncmp(args[1], "-n", 2) == 0)
    {
        i++;
        while (args && args[i])
            printf("%s", args[i++]);
    }
    else
    {
        while(args && args[i])
        {
            printf("%s", args[i++]);
            if (args[i])
                printf(" ");
        }
        printf("\n");
    }
    return (0);
}
