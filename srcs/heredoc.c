#include "../includes/shell.h"

static char *get_heredoc_dem(char *token);

t_cmd *handle_heredoc(t_shell *mini, t_cmd *cmd, char *token)
{
	int fd;
	char *pmpt = NULL, *dem;
	char *str = ft_strchr(token, '<');
	char *temp = ft_strnmdup(token, 0, str - token);
	char *new_token = ft_strtrim(temp, " \f\n\t\v\r");
	cmd->type = set_command_type(token);
	cmd->command = set_path_name(mini, new_token);
	cmd->filename = ft_strdup(CACHE);
	cmd->cmd = get_command(new_token);
	cmd->num_args = get_num_args(new_token);
	cmd->args = set_arg_array(cmd->num_args, new_token, cmd->command);
	cmd->next = NULL;
	dem = get_heredoc_dem(token);
	if((fd = open(CACHE, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
	{
		perror("Failed to cached the input!");
		exit(1);
	}
	while((pmpt = readline(">")) != NULL)
	{
		if(ft_strncmp(dem, pmpt, ft_strlen(dem)) == 0)
		{
			free(pmpt);
			break;
		}
		write(fd, pmpt, ft_strlen(pmpt));
		write(fd, "\n", 2);
		free(pmpt);
	}
	close(fd);
	return(cmd);
}

static char *get_heredoc_dem(char *token)
{
	int i = 0;
	char *str = ft_strchr(token, '<'), *dem;
	while(str && str[i] && str[i] == '<')
		i++;
	while(str && str[i] && ft_isspace(str[i]))
		i++;
	dem = ft_strnmdup(str, i, ft_strlen(str));
	return (dem);
}

