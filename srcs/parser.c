#include "../includes/shell.h"

void expand(t_shell *mini, t_list *list);
int get_num_args(char *token);
void get_args(char **args, char *token, int size);
char *get_command(char *token);
char *set_path_name(t_shell *mini, char *token);
char **set_arg_array(int num_args, char *token, char *cmdpath);

int parse_and_expand(t_shell *mini)
{
	expand(mini, mini->tokens);
	if (mini->cmds->cmd)
	{
		if(!mini->cmds->command || builtin_cmd(mini->cmds->cmd))
		{
			if (check_builtin(mini) == 2)
				return (2);
			else
				return (1);
		}
	}
	return (0);
}
void handle_dollar(t_list *list, t_shell *mini)
{
    t_list *current;
    char 	*token;
    char 	*expanded_token;
    int 	i;
	int		j;
	int 	var_start;
	int 	var_len;
	char 	*var_value;
	int		k;

	current = list;
	token = current->token;
    expanded_token = malloc(strlen(token) + 1);
	i = 0;
	j = 0;
    if (!expanded_token)
        return ;
    while (token[i]) {
        if (token[i] == '$') {
            i++;
	 	var_start = i;
        while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
            i++;
        var_len = i - var_start;
        char var_name[var_len + 1];
        ft_strlcpy(var_name, token + var_start, var_len + 1);
		var_value = extract_env_value(mini->initenv, var_name);
		k = 0;
        while (var_value[k])
            expanded_token[j++] = var_value[k++];
        } else
            expanded_token[j++] = token[i++];
    }
    expanded_token[j] = '\0';
	free(current->token);
	expanded_token = ft_strtrim(expanded_token, " ");
	current->token = expanded_token;
}

void expand(t_shell *mini, t_list *list)
{
	t_list *current = list;
	t_cmd *cmd = NULL;

	while(current)
	{
		handle_dollar(current, mini);
		if(check_if_quoted(current->token))
			cmd = handle_quoted(mini, current->token);
		else if(ft_strchr(current->token, '>'))
			cmd = handel_output(mini, current->token);
		else if(ft_strchr(current->token, '<'))
			cmd = handel_input(mini, current->token);
		else
			cmd = handel_pipe(mini, current);
		mini->cmds = list_add_command(mini->cmds, cmd);
		mini->num_cmds++;
		cmd = NULL;
		current = current->next;
	}
}

t_cmd *handel_pipe(t_shell *mini, t_list *current)
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
	if(!cmd)
			return (NULL);
	cmd->type = set_command_type(current->token);
	cmd->cmd = get_command(current->token);
	cmd->command = set_path_name(mini, current->token);
	cmd->filename = NULL;
	cmd->num_args = get_num_args(current->token);
	cmd->args = set_arg_array(cmd->num_args, current->token, cmd->command);
	cmd->next = NULL;
	return (cmd);
}

char **set_arg_array(int num_args, char *token, char *cmdpath)
{
	char **args = malloc(sizeof(char*) * (num_args + 1));
	if(!args)
		return (NULL);
	args[0] = cmdpath;
	if(num_args > 1)
		get_args(args, token, num_args);
	args[num_args] = NULL;
	return (args);
}

char *set_path_name(t_shell *mini, char *token)
{
	char	**path_dirs;
	char	*command;
	char	*path;
	int		i;

	i = 0;
	command = ft_substr(token, 0, ft_strchr(token, ' ') - token);
	path = command;
	if (access(path, X_OK) == 0)
		return (path);
	else
		path = NULL;
	char *path_value = extract_env_value(mini->initenv, "PATH");
	if (!path_value)
	{
    	if (!builtin_cmd(command))
    	    printf("%s: command not found\n", command);
   		free(command);
   		return (NULL);
	}
	path_dirs = ft_split(path_value, ':');
	if (!path_dirs)
    	return (NULL);
	while (path_dirs && path_dirs[i])
	{
		path = ft_strjoin(path_dirs[i], "/");
		path = ft_strjoin(path, command);
		if (!path)
			return (NULL);
		if (access(path, X_OK) == 0)
			return (path);
		i++;
	}
	if (access(path, X_OK) == -1)
	{
		if (builtin_cmd(command))
			return (NULL);
		else
			printf("%s: command not found\n", command);
	}
	return (NULL);
}

char *get_command(char *token)
{
	int i = 0, j = 0;
	char *command;
	while(token && token[i])
	{
		if(token[i] == ' ')
		{
			command = malloc(sizeof(char)*(i + 1));
			if(!command)
				return (NULL);
			while(token && token[j] && j < i)
			{
				command[j] = token[j];
				j++;
			}
			command[j] = '\0';
			return (command);
		}
		i++;
	}
	if(token[i] == '\0')
		return (token);
	return (NULL);
}

void get_args(char **args, char *token, int size)
{
	int i = 0, k = 0, start = 0, step = 1;
	while(token && token[i] && token[i] != ' ')
		i++;
	while(step < size)
	{
		while(token && token[i] && token[i] == ' ')
			i++;
		start = i;
		while(token && token[i] && token[i] != ' ')
			i++;
		args[step] = malloc(sizeof(char) * (i - start + 1));
		k = 0;
		while(token && token[k + start] && (k + start) < i)
		{
			args[step][k] = token[k + start];
			k++;
		}
		args[step][k] = '\0';
		step++;
	}
}

int get_num_args(char *token)
{
	int i = 0, count = 1, flag = 0;
	while(token && token[i])
	{
		if(token[i] == ' ' && !flag)
		{
			count++;
			flag = 1;
		}
		else if(token[i] != ' ' && flag)
			flag = 0;
		i++;
	}
	return (count);
}

