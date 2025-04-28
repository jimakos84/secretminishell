#include "../includes/shell.h"

char *set_filename(char *token, int ch);
char *set_arg_string(char *token, int ch);
char *get_arg_string(char *token);
int set_command_type(char *token);

t_cmd *handel_output(t_shell *mini, char *token)
{
	/**
	 * example of the command format
	 * echo "the string need to write in to the file" > file.txt
	*/
	printf("output happens\n");
	char *arg_str = set_arg_string(token, '>');
	t_cmd *cmd = malloc(sizeof(t_cmd));
	if(!cmd)
		return (NULL);
	cmd->type = set_command_type(token);
	//cmd->cmd = get_command(arg_str);
	cmd->command = set_path_name(mini, cmd->cmd);
	cmd->filename = set_filename(token, '>');
	//cmd->num_args = get_num_args(arg_str);
	//cmd->args = set_arg_array(cmd->num_args, arg_str, cmd->command);
	cmd->next = NULL;
	free(arg_str);
	return (cmd);
}

int set_command_type(char *token)
{
	if (!token)
		return (0);

	char *trimmed = ft_strtrim(token, " \f\n\t\v\r");

	if (contains_unquoted_char(trimmed, '>'))
	{
		// Look for >> only if both > are unquoted
		if (ft_strnstr(trimmed, ">>", ft_strlen(trimmed)) &&
			contains_unquoted_char(trimmed, '>') &&
			contains_unquoted_char(trimmed + 1, '>'))
			return (APRD_CMD);
		return (OPRD_CMD);
	}
	else if (contains_unquoted_char(trimmed, '<'))
	{
		if (ft_strnstr(trimmed, "<<", ft_strlen(trimmed)) &&
			contains_unquoted_char(trimmed, '<') &&
			contains_unquoted_char(trimmed + 1, '<'))
			return (HDRD_CMD);
		return (IPRD_CMD);
	}
	return (SMPL_CMD);
}


char *set_filename(char *token, int ch)
{
	int i = 1, start = 0;
	char *str = ft_strchr(token, ch), c, *res;
	while(str && str[i] && str[i] == ch)
		i++;
	while(str && str[i] && ft_isspace(str[i]))
		i++;
	start = i;
	if(str[i] == '\'' || str[i] == '"')
	{
		c = str[i];
		i++;
		while(str && str[i] && str[i] != c)
			i++;
		i++;
	}
	else
	{
		while(str && str[i] && !ft_isspace(str[i]))
			i++;
	}
	res = ft_strnmdup(str, start, i);
	return (res);
}

char *set_arg_string(char *token, int ch)
{
	char *s1 = NULL, *s2 = NULL, *s3 = NULL ,*s4 = NULL, *s5 = NULL;
	s1 = ft_strchr(token, ch);
	s2 = ft_strnmdup(token, 0, s1 - token);
	if(s2)
	{
		s3 = ft_strtrim(s2, " \f\n\t\v\r");
		s4 = get_arg_string(s1);
		s5 = ft_strjoin(s3, s4);
		free(s2);
		free(s3);
		free(s4);
	}
	else
	{
		s4 = get_arg_string(s1);
		s5 = ft_strjoin(s2, s4);
		free(s3);
		free(s4);
	}
	return (s5);
}

char *get_arg_string(char *token)
{
	int i = 0;
	char *res = NULL;
	while(token && token[i] && ft_isspace(token[i]))
		i++;
	while(token && token[i] && !ft_isspace(token[i]))
		i++;
	while(token && token[i] && ft_isspace(token[i]))
		i++;
	while(token && token[i] && !ft_isspace(token[i]))
		i++;
	if(!token[i])
		return (NULL);
	res = ft_strnmdup(token, i, ft_strlen(token));
	return (res);
}


