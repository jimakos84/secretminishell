#include "../includes/shell.h"

t_cmd *handel_input(t_shell *mini, char *token)
{
	/**
	 * example of the command format
	 * cat < file.txt
	*/
	t_cmd *cmd = malloc(sizeof(t_cmd));
	if(!cmd)
		return (NULL);
	char *str = ft_strchr(token, '<');
	if(str && str[1] && str[1] == '<')
		return(handle_heredoc(mini, cmd, token));
	char *arg_str = set_arg_string(token, '<');
	cmd->type = set_command_type(token);
	cmd->command = set_path_name(mini, arg_str);
	cmd->filename = set_filename(token, '<');
	cmd->num_args = get_num_args(arg_str);
	cmd->args = set_arg_array(cmd->num_args, arg_str, cmd->command);
	cmd->next = NULL;
	free(arg_str);
	return (cmd);
}
