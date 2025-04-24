#include "../includes/shell.h"

/**
 * creating new env list node 
*/
t_env	*new_node(char *content)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->name = ft_substr(content, 0, (ft_strchr(content, '=') - content));
	if (!node->name)
		return (NULL);
	node->value = ft_strdup(ft_strchr(content, '=') + 1);
	if (!node->value)
		return (NULL);
	node->next = NULL;
	return (node);
}

/**
 * apending a env node to the existing list
*/
void	add_to_list(t_env **env, char *content)
{
	t_env	*new;
	t_env	*current;

	new = new_node(content);
	if (!new)
		return ;
	if (*env == NULL)
		*env = new;
	else
	{
		current = *env;
		while (current && current->next)
			current = current->next;
		current->next = new;
	}
}

/**
 * travesring through the original envp array and pass one item at a time to add_to_list funtion 
*/
void	list_env(t_env **env, char **envp)
{
	while (envp && *envp)
        add_to_list(env, *envp++);
}

/**
 * creating a copy of envp 2D array
*/
char	**copy_env(t_env *env)
{
	int		len;
	int		i;
	char	**copy;
	t_env	*tmp;

	tmp = env;
	len = ft_lst_len(env);
	copy = (char **)malloc((len + 1) * sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	while (tmp)
	{
		if (tmp->name)
		{
			if (tmp->value)
			{
				copy[i] = ft_strdup(tmp->name);
				copy[i] = ft_strjoin(copy[i], "=");
				copy[i] = ft_strjoin(copy[i], tmp->value);
			}
			else
				copy[i] = ft_strdup(tmp->name);
		}	
		tmp = tmp->next;
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

/**
 * custome implementaion of getenv() library function
*/

char	*extract_env_value(t_initenv *initenv, char *name)
{
	t_env	*temp;

	temp = initenv->env;
	while (temp)
	{
		if (ft_strncmp(name, temp->name, sizeof(name)) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return ("");
}

/**
 * memory clering in env linkrd list
*/
// static void	_free_env(t_shell *mini)
// {
// 	t_env	*env;
// 	t_env	*next;
// 	int		i;

// 	i = 0;
// 	while (mini->copy_env && mini->copy_env[i])
// 		free(mini->copy_env[i++]);
// 	if (mini->copy_env)
// 		free(mini->copy_env);
// 	env = mini->env;
// 	while (env != NULL)
// 	{
// 		next = env->next;
// 		if (env->name)
// 			free(env->name);
// 		if (env->value)
// 			free(env->value);
// 		free(env);
// 		env = next;
// 	}
// 	mini->env = NULL;
// 	if (mini->trim)
// 		free(mini->trim);
// 	while (mini->tokens && *mini->tokens)
// 		free(*mini->tokens++);
// 	if (mini->tokens)
// 		free(*mini->tokens);
// 	free(mini);
// }