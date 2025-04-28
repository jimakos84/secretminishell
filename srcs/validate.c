#include "../includes/shell.h"

int input_validate(char **input);
static char *input_preprocess(char **input);
static int check_properly_enclosed(char *input);
static int check_redirect_syntax(char *input, char redirect_char);
static char *remove_comments(char *input);

int input_validate(char **input)
{
	char		*trimmed;

    trimmed = input_preprocess(input);
    if (check_properly_enclosed(trimmed))
        return (syntax_error(trimmed, "minishell: syntax error: unclosed quotes", 2));
    if (check_redirect_syntax(trimmed, '>'))
        return (syntax_error(trimmed, "minishell: syntax error near token 'newline'", 2));
    if (check_redirect_syntax(trimmed, '<'))
        return (syntax_error(trimmed, "minishell: syntax error near token 'newline'", 2));
    free(trimmed);
	return (0);
}

static char *remove_comments(char *input)
{
    char *comment_start;
    char *clean_input;

    comment_start = ft_strchr(input, '#');
    if (comment_start)
        clean_input = ft_strnmdup(input, 0, comment_start - input);
    else
        clean_input = ft_strdup(input);
    return (clean_input);
}

static char *input_preprocess(char **input)
{
    char *no_comments;
    char *trimmed;
    
    no_comments = remove_comments(*input); // always allocates new string
    trimmed = ft_strtrim(no_comments, " \f\n\r\t\v"); // allocates
    free(no_comments);
    return (trimmed); // caller must free
}


static int check_properly_enclosed(char *input)
{
    int i = 0, in_single = 0, in_double = 0;

    while (input && input[i])
    {
        if (input[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (input[i] == '"' && !in_single)
            in_double = !in_double;
        i++;
    }
    return (in_single || in_double);
}

static int check_redirect_syntax(char *input, char redirect_char)
{
    int i = 0;

    while (input && input[i])
    {
        if (input[i] == redirect_char)
        {
            i++;
            if (input[i] == redirect_char)
                i++;
            while (input[i] && ft_isspace(input[i]))
                i++;
            if (!input[i] || input[i] == redirect_char)
                return (1);
        }
        else
            i++;
    }
    return (0);
}
