/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 05:39:43 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/29 03:36:56 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../ft_libft/libft.h"
# include "../ft_libft/ft_printf.h"
# include "../ft_libft/get_next_line.h"

# define SMPL_CMD 1 // For simple command
# define OPRD_CMD 2 // For output redirect command ">"
# define APRD_CMD 3 // For output redirect append command ">>"
# define IPRD_CMD 4 // For input redirect command "<"
# define HDRD_CMD 5 // For input heredoc redirect command "<<"

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
	int				prev_heredoc;
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
	int		last_status;
}	t_initenv;

/**
 * data node strucutre support redircetion handling
*/
typedef struct s_redir
{
	int				type;
	int				was_quoted;
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
	int			_stdin;
	int			_stdout;
	pid_t		*pids;
}	t_shell;

/**
 * Validation
*/
int		input_validate(char **input, t_initenv *env);
int		check_expansion(char *input, t_initenv *env, int i);
int		check_expansion2(char *input, t_initenv *env, int i);
int		validation_for_onechar(char *input);
int		validation_for_twochar(char *input);
int		validation_for_threechar(char *input);
char	*input_preprocess(char **input);
void	remove_comments(char **input);

/**
 * Tokenization
*/
int		extract_tokens(t_list **tokens, char *input);
char	*handle_token(const char *input, int *index);

/**
 * Parsing and expanding
*/
int		set_type(char *token);
int		check_for_pipe_token(t_list *node);
int		parse_and_expand(t_shell *mini);
int		count_args(t_list *tokens);
int		ft_is_in_single(const char *str, int n);
char	*extract_value(t_shell *mini, char *token, int *index);
char	*find_cmd(t_shell *mini, t_list *tokens);
char	**allocate_args_array(int num_args);
char	*set_path_name(t_shell *mini, char *token);
char	*copy_var_value(t_shell *mini, char *token, int *index);
char	*expand_token(char *token, t_shell *mini);
void	parse_commands(t_shell *mini, t_list *tokens);
void	process_token_expansion(t_list *current, t_shell *mini);
void	handle_dollar(t_list *list, t_shell *mini);
void	expansion_preprocess(t_list *tokens);
t_list	*handle_arg_or_redirection(t_cmd *cmd, t_list *current, int *i);
t_list	*fill_args_and_cmd(t_cmd *cmd, t_list *tokens, t_shell *mini);
t_list	*init_cmd_from_token(t_cmd *cmd, t_list *tokens, t_shell *mini, int *i);
t_list	*process_redirections(t_cmd *cmd, t_list *tokens, int *i);
t_cmd	*allocate_cmd_node(void);
t_cmd	*set_cmd(t_list *token);

/**
 * Utility functions
*/
int		ft_strnmcpy(char **dest, char *src, int n, int m);
int		ft_arraylen(char **envp);
int		ft_isempty(char *str);
int		ft_isspace(int c);
int		quotes_checker(char *input, int len);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_getpid(void);
int		ft_rand(void);
char	*random_filename(void);
char	*set_cache_file_name(void);
char	*ft_strnmdup(char const *src, int n, int m);
char	*ft_strjoin_free(char *s1, char *s2);
void	free_env(char **env);
bool	builtin_cmd(char *cmd);
bool	is_redir_or_pipe(char c);
bool	is_numerical(char *str);

/**
 * Quote Utility functions
*/
int		check_if_quoted(const char *input);
int		enclosed_in_quotes(const char *str);
void	set_single_quote_flags(t_list *tokens);
void	remove_quotes_inplace(char *str);
bool	contains_unquoted_char(char *str, char symbol);

/**
 * Signale handle functions
*/
void	init_sig(void);
void	handler(int sig);
void	init_sig_heredoc(void);
void	handler_heredoc(int sig, siginfo_t *info, void *context);

/**
 * Redirect handle functions
*/
int		handle_redirections(t_cmd *current);
int		check_filename(t_redir *r);
int		close_fds(int fd[][2], int limit);
int		handle_output(t_redir *r, int fd);
int		handle_append(t_redir *r, int fd);
int		handle_input(t_redir *r, int fd);
int		handle_heredoc(t_redir *r, int fd);
int		heredoc_interaction(t_shell *mini, t_redir *r, int *fd);
int		execute_heredoc(t_shell *mini, t_redir *r, int fd);
int		preprocessing_heredocs(t_shell *mini);
int		check_heredoc_no_cmd(t_shell *mini, int last_exit_status);
int		handle_child_process(t_shell *mini, t_redir *r, int fd, char *cache);
int		handle_heredoc_status(int status, char *cache, int fd, t_redir *r);
char	*set_cache_file_name(void);
void	heredoc_cleaner(t_shell *mini);

/**
 * Helper functions
*/
int		ft_lst_len(t_env *env);
int		is_redirection_token(char *token);
int		is_valid_identifier_len(const char *name, int len);
char	*string_build(char *s1, char *s2);
char	*string_build2(char *s1, char *s2);
char	**get_path_values(t_shell *mini);
void	add_redir(t_redir **list, t_redir *new);
bool	is_invalid_pipe_sequence(t_list *current);
t_redir	*create_redir_node(int type, const char *filename);

/**
 * Built in export
*/
int		builtin_export(t_shell *mini);
int		echo_error(char *msg, char *oldpwd);
int		change_env_value(t_env *env, char *var);

/**
 * Error handle in general
*/
int		syntax_error(char *input, int code);
void	check_stat(char *path, t_shell *mini, int **fd);
void	perror_exit(const char *msg);
void	stat_error(char *path, char c, t_shell *mini, int **fd);
void	p_exe_error(char *command, int err, t_initenv *initenv);

/**
 * Cleaner funtions
*/
int		clear_and_exit(t_shell *mini);
int		clear_commands(t_cmd *cmds);
int		clear_tokens(t_list *tokens);
int		clear_array(char **array);
int		cleanup_and_exit(t_shell *mini);
void	clear_env(t_env *env);
void	free_redirections(t_redir *redir_list);
void	clear_path_dirs(char **path_dirs, int i);

/**
 * Built_in funtions
*/
int		print_cd_error(char *path, char *oldpwd);
int		try_change_dir(t_shell *mini, char *target, char *oldpwd);
int		builtin_cd(t_shell *mini);
int		check_builtin(t_cmd *current, t_shell *mini);
int		builtin_unset(t_shell *mini);
int		builtin_env(t_shell *mini);
int		builtin_pwd(t_initenv *env);
int		home_not_set(char *oldpwd);
void	updatewd(t_shell *mini, char *newpwd, char *oldpwd);
void	remove_env_nodes(t_shell *mini, char *unset);
void	exit_builtin(t_cmd *cmd, t_shell *mini, int **fd);

/**
 * Built_in echo
*/
int		builtin_echo(t_cmd *cmd);
void	print_args(char **args, int *index);

/**
 * Handle enviornment
*/
char	**copy_env(t_env *env);
char	*extract_env_value(t_initenv *initenv, char *name);
void	add_to_list(t_env **env, char *content);
void	list_env(t_env **env, char **envp);
t_env	*new_node(char *content);

/**
 * Built in exit
*/
int		check_valid_value(char *value);
int		handle_too_many_args(t_shell *mini);
int		validate_exit_argument(t_shell *mini, char *arg);
int		exit_mini(t_shell *mini);
void	exit_proccedure(t_shell *mini);

/**
 * Execution funtions
*/
int		**alloc_fds(int limit);
int		init_pipes(int **fd, int limit);
int		execute(t_shell *mini);
int		handle_builtin(t_shell *mini, t_cmd *current);
int		wait_for_children(int n, t_shell *mini, pid_t *pids);
int		execution(t_shell *mini, t_cmd *cmd, int **fd);
int		activate_shell(char *input, t_initenv *env);
int		ft_isquoted(const char *str, int n);
void	free_fds(int **fd, int count);
void	close_fds2(int **fd, int limit);
void	pre_execute(t_shell *mini, t_cmd *cmd, int **fd);
pid_t	*run_commands(t_shell *mini, t_cmd *current, int **fd, int *index);
pid_t	execute_command(t_shell *mini, t_cmd *cmd, int **fd, int index);
t_cmd	*list_add_command(t_cmd *cmds, t_cmd *node);

/**
 * Debugging tool funtions
*/
int		print_cmd(t_cmd *cmd);
int		print_token(t_list *token);
int		print_all(t_shell *mini);
#endif
