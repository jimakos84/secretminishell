#include "../includes/shell.h"

t_list *list_add_back(t_list *list, char *str);

int extract_tokens(t_list **tokens, char *input)
{
	int i = 0;
	
	/**
	 * first trying to extract a token bas on space/(' ')/ASCII INT 32 character
	*/
	char *line = ft_strtrim(input, " ");
	char **words = ft_split(line, '|');
	while(words && words[i])
	{
		*tokens = list_add_back(*tokens, ft_strtrim(words[i], " "));
		i++;
	}
	return (0);
}

t_list *list_add_back(t_list *list, char *str)
{
	t_list *node = malloc(sizeof(t_list)), *current;
	if(!node)
		return (NULL);
	node->next = NULL;
	node->token = ft_strdup(str);
	if(!list)
		return (node);
	current = list;
	while(current && current->next)
		current = current->next;
	current->next = node;
	return (list);
}

t_cmd *list_add_command(t_cmd *cmds, t_cmd *node)
{
	t_cmd *current;
	if(!node)
		return (cmds);
	if(!cmds)
		return (node);
	current = cmds;
	while(current && current->next)
		current = current->next;
	current->next = node;
	return (cmds);
}
