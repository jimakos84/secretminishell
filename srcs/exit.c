/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:59:31 by dvlachos          #+#    #+#             */
/*   Updated: 2025/05/05 15:59:50 by dvlachos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

static int  check_valid_value(char *value)
{
    int     exit_value;

    exit_value = ft_atoi(value);
    exit_value = exit_value % 256;
    return (exit_value);
}

static void     exit_proccedure(t_shell *mini)
{
    int     status;

    status = 0;
    clear_env(mini->initenv->env);
    free(mini->initenv->home);
    status = mini->initenv->last_status;
    free(mini->initenv);
    clear_and_exit(mini);
    rl_clear_history();
    exit (status);
}

int        exit_mini(t_shell *mini)
{
    int     i;
    char    *arg;
    int     status;

    i = 0;
    if (mini->cmds->args[1] && mini->cmds->args[2])
    {
        mini->initenv->last_status = 1;
        ft_putstr_fd("minishell: exit: ", 2);
        ft_putendl_fd("too many arguments", 2);
        exit_proccedure(mini);
    }
    if (mini->cmds->args[1])
    {
        arg = mini->cmds->args[1];
        while (arg[i])
        {
            if (ft_isdigit(arg[i]) || arg[i] == '-' || arg[i] == '+')
                i++;
            else
            {
                mini->initenv->last_status = 2;
                printf("exit\n");
                ft_putstr_fd("minishell: exit: ", 2);
                ft_putstr_fd(arg, 2);
                ft_putendl_fd(": numeric argument required", 2);
                exit_proccedure(mini);
                break ;
            }
        }
        mini->initenv->last_status = check_valid_value(mini->cmds->args[1]);
    }
    if (!mini->cmds->args[1])
        printf("exit\n");
    clear_env(mini->initenv->env);
    free(mini->initenv->home);
    status = mini->initenv->last_status;
    free(mini->initenv);
    clear_and_exit(mini);
    rl_clear_history();
    exit (status);
}
