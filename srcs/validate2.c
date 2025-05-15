/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:22:33 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/15 05:50:51 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
* Function declaration of helper fuctions
*/
char	*input_preprocess(char **input);
void	remove_comments(char **input);

/*
* Removes shell-style comments from the input string.
* A comment is considered to start at the first '#' character
* and continues to the end of the line.
* If no comment is found, the original string is duplicated.
*
* Parameters:
* - input: The input command line string.
*
* Returns:
* - A newly allocated string with the comment (if any) removed.
*/

void	remove_comments(char **input)
{
	int		i;
	char	*str;
	char	*result;

	if (!input || !*input || !ft_strchr(*input, '#'))
		return ;
	i = 0;
	str = *input;
	while (str[i])
	{
		if (str[i] == '#' && !ft_isquoted(str, i))
			break ;
		i++;
	}
	result = (char *)malloc(i + 1);
	if (!result)
		return ;
	ft_memcpy(result, str, i);
	result[i] = '\0';
	free(str);
	*input = result;
}

/*
* Preprocesses the input string by removing comments and trimming
* whitespace (defined as ISSPACES3 in the header file)
* - Removes comments using `remove_comments`.
* - Trims leading and trailing whitespace using `ft_strtrim`.
*
* Parameters:
* - input: A pointer to the original input string.
*
* Returns:
* - A newly allocated, cleaned-up version of the input.
*/

char	*input_preprocess(char **input)
{
	char	*trimmed;

	trimmed = ft_strtrim(*input, ISSPACE3);
	return (trimmed);
}

int	check_expansion(char *input, t_initenv *env, int i)
{
	char	var[1024];
	int		j;
	t_env	*envi;

	envi = env->env;
	j = 0;
	i++;
	while (input[i] && input[i] != ' ')
		var[j++] = input[i++];
	var[j] = '\0';
	if (is_valid_identifier_len(var, j))
	{
		j = 0;
		while (envi)
		{
			if (ft_strncmp(var, envi->name, ft_strlen(envi->name)) == 0)
				return (0);
			envi = envi->next;
		}
	}
	ft_putstr_fd("minishell: $", 2);
	ft_putstr_fd(var, 2);
	ft_putendl_fd(": ambiguous redirect", 2);
	free(input);
	return (1);
}

int	check_expansion2(char *input, t_initenv *env, int i)
{
	char	var[1024];
	int		j;
	t_env	*envi;

	envi = env->env;
	j = 0;
	i++;
	while (input[i] && input[i] != ' ')
		var[j++] = input[i++];
	var[j] = '\0';
	if (is_valid_identifier_len(var, j))
	{
		j = 0;
		while (envi)
		{
			if (ft_strncmp(var, envi->name, ft_strlen(envi->name)) == 0)
				return (0);
			envi = envi->next;
		}
	}
	ft_putendl_fd("minishell: empty variable between pipes", 2);
	return (1);
}
