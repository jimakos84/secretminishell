/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:57:12 by dvlachos          #+#    #+#             */
/*   Updated: 2025/04/29 13:57:14 by dvlachos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

void    change_env_value(t_env *env, char *var)
{
    t_env   *current;
    char    *env_var;

    if (!is_valid_identifier(var) && !ft_strchr(var, '='))
    {
        ft_putendl_fd("not a valid identifier", 2);
        return ;
    }
    current = env;
    while (current)
    {
        env_var = current->name;
        if (ft_strncmp(var, env_var, ft_strlen(var)) == 0)
        {
            free(current->value);
            current->value = ft_strdup(ft_strchr(var, '=') + 1);
            return ;
        }
        current = current->next;
    }
    add_to_list(&env, var);
}

void    builtin_export(t_shell *mini)
{
    t_env   *env;
    t_env   *current;
    char    **args;
    int     i;

    i = 1;
    args = mini->cmds->args;
    env = mini->initenv->env;
    current = env;
    if (!args[i])
    {
        while(current)
        {
            printf("declare -x %s=\"%s\"\n", current->name, current->value);
            current = current->next;
        }
    }
    while (args && args[i])
        change_env_value(env, args[i++]);
}
