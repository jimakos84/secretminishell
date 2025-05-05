#ifndef SHELL_H
# define SHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../ft_libft/libft.h"
# include "../ft_libft/ft_printf.h"
# include "../ft_libft/get_next_line.h"
# include <errno.h>

# define SMPL_CMD 1 // For simple command
# define OPRD_CMD 2 // For output redirect command ">"
# define APRD_CMD 3 // For output redirect append command ">>"
# define IPRD_CMD 4 // For input redirect command "<"
# define HDRD_CMD 5 // For input heredoc redirect command "<<"

# define CACHE "cache.txt"
# define ISSPACE3 " \f\n\r\t\v"
# define SPECIALCHARS "()[]{}&;\\"

/**
 * data node strucutre to hold the tokens extract from input, this node will make a linked list
*/
typedef struct s_list
{
	char 			*token;
	int				in_single_quotes;
	struct s_list 	*next;
} t_list;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_initenv
{
	char	**copy_env;
	t_env	*env;
	char	*home;
	int		last_status;
}	t_initenv;

typedef struct s_redir {
	int type;
	char *filename;
	struct s_redir *next;
} t_redir;


typedef struct s_cmd
{
	int 			type;
	int				is_builtin;
	char 			*cmd;
	char 			*command;
	char 			**args;
	int 			num_args;
	t_redir			*redir_list;
	struct s_cmd 	*next;
} t_cmd;

typedef struct s_shell
{
	int 		num_cmds;
	t_cmd 		*cmds;
	t_list 		*tokens;
	t_initenv	*initenv;
	int			status;
} t_shell;


int	ft_isquoted(const char *str, int n);
/**
 * Implementaion in main.c
*/
void 	print(t_list *list, char *msg);
void		printcmdtokens(t_shell *mini);
/**
 * Implementaion in srcs/validate.c
*/
int 	input_validate(char **input);
//char 	*in_quotes(char *input);

/**
 * Implementaion in srcs/error.c
*/
int 	syntax_error(char *input, char *msg, int code);
void	p_exe_error(char *command, int err, t_initenv *initenv);
void	perror_exit(const char *msg);

/**
 * Implementation in srcs/helper.c
*/
int 	activate_shell(char *input, t_initenv *env);

/**
 * Implementation in srcs/token.c
*/
t_list 	*list_add_back(t_list *list, char *str);
t_cmd 	*list_add_command(t_cmd *cmds, t_cmd *node);
int 	extract_tokens(t_list **tokens, char *input);

/**
 * Implementation in srcs/parser.c
*/
int 	parse_and_expand(t_shell *mini);
t_cmd 	*handel_pipe(t_shell *mini, t_list *current);

/**
 * Implementaion in srcs/utils.c
*/
int 	ft_strnmcpy(char **dest, char *src, int n, int m);
char 	*ft_strnmdup(char const *src, int n , int m);

/**
 * Implementaion in srcs/execute.c
*/
int 	execute(t_shell *mini);

/*
 * Implementaion in srcs/cleaner.c
*/
int 	clear_and_exit(t_shell *mini);
int		clear_commands(t_cmd *cmds);
int		clear_tokens(t_list *tokens);
int		clear_array(char **array);
void 	clear_env(t_env *env);



/*
 * Implementaion in srcs/signal.c
*/
void 	init_sig(void);

/*
 * Implementaion in srcs/redirect.c
*/

int		handle_redirections(t_cmd *current);
int		close_fds(int fd[][2], int limit);

/*
 * Implementaion in srcs/input.c
*/
t_cmd 		*handel_output(t_shell *mini, char *token);
t_cmd 		*handel_input(t_shell *mini, char *token);
void 		add_redir(t_redir **list, t_redir *new);
t_redir 	*create_redir_node(int type, const char *filename);




/**
 * Implementaion in srcs/heredoc.c
*/
t_cmd 	*handle_heredoc(t_shell *mini, t_cmd *cmd, char *token);

t_env	*new_node(char *content);
void	add_to_list(t_env **env, char *content);
void	list_env(t_env **env, char **envp);
char	**copy_env(t_env *env);
char	*extract_env_value(t_initenv *initenv, char *name);
int		builtin_cd(t_shell *mini);
int		builtin_env(t_shell *mini);
int		check_builtin(t_shell *mini);
int		ft_arraylen(char **envp);
int		ft_isempty(char *str);
char 	*get_command(char *token);
int		tokenize(t_shell *mini, char *input);
int		quotes_checker(char *input, int len);
bool	builtin_cmd(char *cmd);
void    builtin_unset(t_shell *mini);
int 	is_redirection_token(char *token);
int		builtin_export(t_shell *mini);
int		ft_isspace(int c);
int	is_valid_identifier_len(const char *name, int len);
t_cmd 	*handle_quoted(t_shell *mini, char *token);
bool 	contains_unquoted_char(char *str, char symbol);
int 	check_if_quoted(const char *input);
void 	remove_quotes_inplace(char *str);
char 	*set_path_name(t_shell *mini, char *token);
int 	get_num_args(char *token);
char 	**set_arg_array(int num_args, char *token, char *cmdpath);
int		ft_lst_len(t_env *env);
char 	**quote_aware_tokenize(const char *input);
void 	free_env(char **env);
bool 	is_invalid_pipe_sequence(t_list *current);
int     builtin_echo(t_shell *mini);
void 	handle_dollar(t_list *list, t_shell *mini);
int		enclosed_in_quotes(const char *str);
void	set_single_quote_flags(t_list *tokens);
int     exit_mini(t_shell *mini);

#endif
