#include "../includes/shell.h"

static void    remove_env_nodes(t_shell *mini, char *unset)
{
    t_env   *current;
    t_env   *prev;

    current = mini->initenv->env;
    prev = NULL;
    if (!unset)
        return ;
    while (current)
    {
        if (ft_strncmp(current->name, unset, ft_strlen(unset)) == 0)
        {
            if (prev)
                prev->next = current->next;
             else
                mini->initenv->env = current->next;
            free(current->name);
            if (current->value)
              free(current->value);
            free(current);
            return ;
        }
        prev = current;
        current = current->next;
    }
}

void    builtin_unset(t_shell *mini, char **unset)
{
    int     i;

    i = 1;
    if (!unset)
        return ;
    while (unset[i])
        remove_env_nodes(mini, unset[i++]);
}