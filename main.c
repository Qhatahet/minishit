/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:41:23 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/30 00:51:23 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_type(t_type type)
{
	switch (type)
	{
	case COMMAND:
		printf("COMMAND\n");
		break ;
	case ARGUMENT:
		printf("ARGUMENT\n");
		break ;
	case HEREDOC:
		printf("HEREDOC\n");
		break ;
	case APPEND:
		printf("APPEND\n");
		break ;
	case TEXT:
		printf("TEXT\n");
		break ;
	case REDIRECTIN:
		printf("REDIRECTIN\n");
		break ;
	case REDIRECTOUT:
		printf("REDIRECTOUT\n");
		break ;
	case ENDOFFILE:
		printf("ENDOFFILE\n");
		break ;
	case FILENAME:
		printf("FILENAME\n");
		break ;
	}
}

// test the tokenizer
void	print_tokens(t_token *arg)
{
	t_token	*tmp;
	int		i;

	tmp = arg;
	while (tmp)
	{
		i = 0;
		while (tmp->content[i])
		{
			printf("argv[%i] = %s , type = ", i, tmp->content[i]);
			print_type(tmp->type[i]);
			i++;
		}
		tmp = tmp->next;
		if (tmp)
			printf("------->%c\n", '|');
	}
}

int	ft_executor(t_shell *shell, t_token *token)
{
	while (token)
	{
		if (token->content && token->content[0])
		{
			if (ft_strcmp(token->content[0], "env") == 0
				&& token->content[1] == NULL)
			{
				print_env(shell->env);
				return (1);
			}
			else if (ft_strcmp(token->content[0], "echo") == 0)
			{
				echo_command(shell, token);
				return (1);
			}
			else if (ft_strcmp(token->content[0], "export") == 0)
			{
				export_command(shell, token);
				return (1);
			}
			else if (ft_strcmp(token->content[0], "unset") == 0)
			{
				unset_command(shell, token);
				return (1);
			}
			else if (ft_strcmp(token->content[0], "pwd") == 0)
			{
				ft_pwd();
				return (1);
			}
			else if (ft_strcmp(token->content[0], "cd") == 0)
			{
				ft_cd(shell, token);
				return (1);
			}
		}
		token = token->next;
	}
	return (0);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->variable)
			free(env->variable);
		if (env->content)
			free(env->content);
		free(env);
		env = tmp;
	}
	free(env);
}

void	free_tokenizer(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens->next;
		if (tokens->content)
			ft_free_2d(tokens->content);
		if (tokens->type)
			free(tokens->type);
		free(tokens);
		tokens = temp;
	}
	free(tokens);
}

void	init_minishell(t_shell *shell)
{
	t_token		*tokens;
	t_parser	*parser;

	while (1)
	{
		shell->prompt = readline("\033[93mArab Spring 🐣🐥 -> \033[0m");
		if (!shell->prompt)
		{
			printf("exit\n");
			if (shell->env)
				free_env(shell->env);
			free(shell);
			exit(0);
		}
		if (!ft_strcmp(shell->prompt, ""))
			continue ;
		else if (shell->prompt)
		{
			add_history(shell->prompt);
			tokens = tokenizer(shell);
			if (!tokens)
			{
				free(shell->prompt);
				continue ;
			}
			parser = ft_calloc(1, sizeof(t_parser));
			if (!parser)
				exit(EXIT_FAILURE);
			if (tokens)
			{
				ft_parser(tokens, parser, shell);
				ft_expander(shell, tokens);
			}
			// print_tokens(tokens);
			// ft_executor(shell, tokens);
			if (tokens && !ft_executor(shell, tokens))
				execute(shell, tokens, parser);
			if (shell->prompt)
				free(shell->prompt);
			if (parser)
				free(parser);
			if (tokens)
				free_tokenizer(tokens);
		}
		else
		{
			// free(shell->prompt);
			exit(0);
		}
		if (shell->enviroment)
		{
			ft_free_2d(shell->enviroment);
			shell->enviroment = NULL;
		}
	}
}

// take the arguments and the enviroment
int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argv;
	// for (int i = 0; envp[i]; i++)
	// 	printf("hello = %s\n", envp[i]);
	// if (envp[0] != NULL)
	// 	env_copy(shell, envp);
	if (argc > 1)
	{
		printf("Error\nwrong number of arguments\n");
		return (1);
	}
	shell = ft_calloc(1, sizeof(t_shell));
	if (!shell)
		exit(EXIT_FAILURE);
	if (envp[0] == NULL)
	{
		shell->env = NULL;
	}
	else
		env_copy(shell, envp);
	signal_handler();
	init_minishell(shell);
	if (shell->env)
	{
		// printf("qais\n");
		free_env(shell->env);
		free(shell->env);
		shell->env = NULL;
	}
	free(shell->prompt);
	free(shell);
	rl_clear_history();
	return (0);
}
