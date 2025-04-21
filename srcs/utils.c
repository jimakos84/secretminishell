#include "../includes/shell.h"

char *ft_strnmdup(char const *src, int n , int m)
{
	int i = 0;
	char *dest;

	if(!src || m == 0)
		return (NULL);
	dest = malloc(sizeof(char) * ((m - n) + 1));
	if(!dest)
		return (NULL);
	while(src && src[i + n] && (i + n) < m)
	{
		dest[i] = src[i + n];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int ft_strnmcpy(char **dest, char *src, int n, int m)
{
	int i = 0;

	if (n >= m || !src || !dest)
		return (-1);

	*dest = malloc(sizeof(char) * (m - n + 1));
	if (!*dest)
		return (-1);

	while ((i + n) < m && src[i + n])
	{
		(*dest)[i] = src[i + n];
		i++;
	}
	(*dest)[i] = '\0';
	return (i);
}

void print_args(char **args, int size)
{
	int i = 0;
	while(i <= size)
	{
		printf("args[%d] : %s\n", i, args[i]);
		i++;
	}
}

void print_cmd(t_cmd *cmd)
{
	t_cmd *current = cmd;
	while(current)
	{
		printf("command : %s\n", current->command);
		printf("args : \n");
		print_args(current->args, current->num_args);
		current = current->next;
	}
}

void print(t_list *list, char *msg)
{
	printf("\n%s\n", msg);
	t_list *current = list;
	while(current)
	{
		printf("%s\n", current->token);
		current = current->next;
	}
	printf("------------------------------------\n");
}

int	ft_arraylen(char **envp)
{
	int		len;

	len = 0;
	while (envp && envp[len])
		len++;
	return (len);
}

int	ft_isempty(char *str)
{
	while (*str)
	{
		if (!ft_isspace(*str))
			return (0);
		str++;
	}
	return (1);
}

int	ft_isspace(int c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v')
		return (1);
	else
		return (0);
}

int	quotes_checker(char *input, int len)
{
	int	in_quote;
	int	i;

	i = 0;
	in_quote = 0;
	while (input && input[i] && i < len)
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			if (in_quote == 0)
				in_quote = input[i];
			else if (in_quote == input[i])
				in_quote = 0;
		}
		i++;
	}
	return (in_quote);
}

bool	builtin_cmd(char *cmd)
{
	static char	*builtins[6];
	int		i;

	i = 0;
	builtins[0] = "exit";
	builtins[1] = "cd";
	builtins[2] = "env";
	builtins[3] = "pwd";
	builtins[4] = "unset";
	builtins[5] = NULL;
	while (builtins[i])
	{
		if (ft_strncmp(cmd, builtins[i], sizeof(cmd)) == 0)
			return (true);
		i++;
	}
	return (false);
}
