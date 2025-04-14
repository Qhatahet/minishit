/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:24:34 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/14 18:39:01 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
#include <sys/types.h>

typedef struct s_env
{
	char				*variable;
	char				*content;
	struct s_env		*next;
}						t_env;

typedef struct s_export
{
	int				index;
	char			*variable;
	char			*content;
	int				flag;
}					t_export;

typedef struct s_parser
{
	int				append_counter;
	int				commands_counter;
	int				heredocs_counter;
	int				redirect_counter;
	int				filename_counter;
	int				arguments_counter;
	int				index;
	int				filename_flag;
	int				command_flag;
}					t_parser;

typedef struct s_shell
{
	char			**paths;
	char			*prompt;
	char			**enviroment;
	int				prompt_index;
	int				token_index;
	int				token_flag;
	int				qoute_flag;
	int				counter;
	int				temp_index;
	int				echo_flag;
	int				quote_counter;
	t_env			*env;
}					t_shell;

typedef enum s_type
{
	TEXT,
	FILENAME,
	COMMAND,
	ARGUMENT,
	HEREDOC,
	APPEND,
	REDIRECTOUT,
	REDIRECTIN
}					t_type;

typedef struct s_token
{
	t_type			*type;
	char			**content;
	struct s_token	*next;
}					t_token;

typedef struct s_expand
{
	int				outer;
	int				inner;
	int				quotes_count;
	int				quote_flag;
	char			quote;
	char			*output;
	char			*variable;
	int				var_length;
	int				single_qoute;
}					t_expand;

// toknizer
t_token				*tokenizer(t_shell *shell);
t_token				*create_new_node(t_shell *shell);
void				copy(t_shell *shell, t_token *token);
void				copy_quotes(t_shell *shell, t_token *token, char detect);
void				tokenizer_size(t_shell *shell);
void				check_quotes(t_shell *shell);
int					check_pipe(t_shell *shell);
int					check_quotes_norm(t_shell *shell, int *quote, int i,
						char c);
void				check_prompt_1(t_shell *shell);
void				check_prompt_2(t_shell *shell, t_token **token);
void				check_prompt_3(t_shell *shell, t_token **token);
void				check_prompt_4(t_shell *shell);
void				check_errors(t_shell *shell);

// parser
void				ft_parser(t_token *head, t_parser *parser, t_shell *shell);
void				detect_heredoc(t_parser *parser, t_token *temp);
void				detect_arguments(t_parser *parser, t_token *temp);
void				detect_command(t_parser *parser, t_token *temp,
						char **paths);
void				detect_redirect(t_parser *parser, t_token *temp);
void				detect_filename(t_parser *parser, t_token *temp);

// expander
void				ft_expander(t_shell *shell, t_token *token);
void				count_quotes(t_token *token, t_expand *expand);
void				expand_dollar(t_shell *shell, t_token *token,
						t_expand *expand);
void				check_env(t_shell *shell, t_expand *expand);
void				copy_var(t_token *token, t_expand *expand);
void				get_length(t_token *token, t_expand *expand);
void				ft_outjoin(t_token *token, t_expand *expand);
void				check_cmd(t_token *token, t_expand *expand, char **paths);
void				quote_remover(t_token *token, t_expand *expand);

// env command
void					env_copy(t_shell *shell, char **env);
void					env_command(t_shell *shell);
void					print_env(t_env *env);
t_env					*create_env_node(void);

// echo command
void					echo_command(t_shell *shell, t_token *token);

//export command
void	ft_free_2d(char **a);
void	sorted_print(t_env *env);
void	export_command(t_shell *shell,t_token *token);
void 	scan_env(t_shell *shell, t_export *export);

// unset command
void	unset_command(t_shell *shell, t_token *token);

// testing
void	execute(t_shell *shell, t_token *tokens);
void	add_backslash(t_shell *shell);
void	get_paths(t_shell *shell);

void	skip_spaces(t_shell *shell);
#endif