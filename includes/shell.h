/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 05:39:43 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/07 05:26:05 by tsomacha         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
 * data node strucutre to hold the tokens extract from input,
 * this node will make a linked list
*/
typedef struct s_list
{
	char			*token;
	int				in_single_quotes;
	struct s_list	*next;
}	t_list;

/**
 * data node strucutre to represent key/value pair in envp
*/
typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

/**
 * data node strucutre to handle envp inside the program
*/
typedef struct s_initenv
{
	char	**copy_env;
	t_env	*env;
	char	*home;
	int		last_status;
}	t_initenv;

/**
 * data node strucutre support redircetion handling
*/
typedef struct s_redir
{
	int				type;
	char			*filename;
	struct s_redir	*next;
}	t_redir;

/**
 * data node strucutre to store all the realted data for a processed command
*/
typedef struct s_cmd
{
	int				num_args;
	int				type;
	int				is_builtin;
	char			*cmd;
	char			*command;
	char			**args;
	t_redir			*redir_list;
	struct s_cmd	*next;
}	t_cmd;

/**
 * main data strucutre which will store all the data within
 * a single execution cycle
*/
typedef struct s_shell
{
	int			num_cmds;
	t_cmd		*cmds;
	t_list		*tokens;
	t_initenv	*initenv;
	int			status;
}	t_shell;

/**
 * Validation
*/
int		input_validate(char **input);
char	*input_preprocess(char **input);
char	*remove_comments(char *input);

/**
 * Tokenization
*/
int		extract_tokens(t_list **tokens, char *input);
char	*handle_token(const char *input, int *index);

/**
 * Parsing and expanding
*/
int		parse_and_expand(t_shell *mini);
char	*string_build(char *s1, char *s2);
char	*extract_value(t_shell *mini, char *token, int *index);
t_list	*handle_arg_or_redirection(t_cmd *cmd, t_list *current, int *i);
t_list	*fill_args_and_cmd(t_cmd *cmd, t_list *tokens, t_shell *mini);
t_cmd	*allocate_cmd_node(void);
char	**allocate_args_array(int num_args);
char	*set_path_name(t_shell *mini, char *token);
char	*copy_var_value(t_shell *mini, char *token, int *index);
char	*expand_token(char *token, t_shell *mini);
int		count_args(t_list *tokens);
int		ft_is_in_single(const char *str, int n);
void	parse_commands(t_shell *mini, t_list *tokens);
void	process_token_expansion(t_list *current, t_shell *mini);
void	handle_dollar(t_list *list, t_shell *mini);
int		set_type(char *token);
t_cmd	*set_cmd(t_list *token);

bool	builtin_cmd(char *cmd);
bool	contains_unquoted_char(char *str, char symbol);
bool	is_invalid_pipe_sequence(t_list *current);
char	*extract_env_value(t_initenv *initenv, char *name);
char	*ft_strnmdup(char const *src, int n, int m);
char	*get_command(char *token);
char	**copy_env(t_env *env);
char	**quote_aware_tokenize(const char *input);
char	**set_arg_array(int num_args, char *token, char *cmdpath);
int		activate_shell(char *input, t_initenv *env);
int		builtin_cd(t_shell *mini);
int		builtin_echo(t_shell *mini);
int		builtin_env(t_shell *mini);
int		builtin_export(t_shell *mini);
int		check_builtin(t_shell *mini);
int		check_if_quoted(const char *input);
int		clear_and_exit(t_shell *mini);
int		clear_array(char **array);
int		clear_commands(t_cmd *cmds);
int		clear_tokens(t_list *tokens);
int		close_fds(int fd[][2], int limit);
int		enclosed_in_quotes(const char *str);
int		execute(t_shell *mini);
int		exit_mini(t_shell *mini);
int		ft_arraylen(char **envp);
int		ft_isempty(char *str);
int		ft_isquoted(const char *str, int n);
int		ft_isspace(int c);
int		ft_lst_len(t_env *env);
int		ft_strnmcpy(char **dest, char *src, int n, int m);
int		get_num_args(char *token);
int		handle_redirections(t_cmd *current);
int		is_redirection_token(char *token);
int		is_valid_identifier_len(const char *name, int len);
int		quotes_checker(char *input, int len);
int		syntax_error(char *input, char *msg, int code);
int		tokenize(t_shell *mini, char *input);
t_cmd	*handel_input(t_shell *mini, char *token);
t_cmd	*handel_output(t_shell *mini, char *token);
t_cmd	*handel_pipe(t_shell *mini, t_list *current);
t_cmd	*handle_heredoc(t_shell *mini, t_cmd *cmd, char *token);
t_cmd	*handle_quoted(t_shell *mini, char *token);
t_cmd	*list_add_command(t_cmd *cmds, t_cmd *node);
t_env	*new_node(char *content);
t_list	*list_add_back(t_list *list, char *str);
t_redir	*create_redir_node(int type, const char *filename);
void	add_redir(t_redir **list, t_redir *new);
void	add_to_list(t_env **env, char *content);
void	builtin_unset(t_shell *mini);
void	clear_env(t_env *env);
void	free_env(char **env);
void	init_sig(void);
void	list_env(t_env **env, char **envp);
void	p_exe_error(char *command, int err, t_initenv *initenv);
void	perror_exit(const char *msg);
void	print(t_list *list, char *msg);
void	printcmdtokens(t_shell *mini);
void	remove_quotes_inplace(char *str);
void	set_single_quote_flags(t_list *tokens);
bool	is_redir_or_pipe(char c);

#endif
