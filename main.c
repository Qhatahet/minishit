/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:41:23 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/18 20:53:07 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	print_type(t_type type)
// {
// 	switch (type)
// 	{
// 	case COMMAND:
// 		printf("COMMAND\n");
// 		break ;
// 	case ARGUMENT:
// 		printf("ARGUMENT\n");
// 		break ;
// 	case HEREDOC:
// 		printf("HEREDOC\n");
// 		break ;
// 	case APPEND:
// 		printf("APPEND\n");
// 		break ;
// 	case TEXT:
// 		printf("TEXT\n");
// 		break ;
// 	case REDIRECTOUT:
// 		printf("REDIRECTOUT\n");
// 		break ;
// 	case REDIRECTIN:
// 		printf("REDIRECTIN\n");
// 		break ;
// 	case FILENAME:
// 		printf("FILENAME\n");
// 		break ;
// 	}
// }

// test the tokenizer
// void	print_tokens(t_token *arg)
// {
// 	t_token	*tmp;
// 	int		i;

// 	tmp = arg;
// 	while (tmp)
// 	{
// 		i = 0;
// 		while (tmp->content[i])
// 		{
// 			printf("argv[%i] = %s , type = ", i, tmp->content[i]);
// 			print_type(tmp->type[i]);
// 			i++;
// 		}
// 		tmp = tmp->next;
// 		if (tmp)
// 			printf("------->%c\n", '|');
// 	}
// }

void	ft_executor(t_shell *shell, t_token *token)
{
	while (token)
	{
		if (token->content && token->content[0])
		{
			if (ft_strcmp(token->content[0], "env") == 0
				&& token->content[1] == NULL)
				print_env(shell->env);
			else if (ft_strcmp(token->content[0], "echo") == 0)
				echo_command(shell, token);
			else if (ft_strcmp(token->content[0], "export") == 0)
				export_command(shell,token);
			else if(ft_strcmp(token->content[0], "unset") == 0)
				unset_command(shell,token);
		}
		token = token->next;
	}
}

void	free_env(t_env *env)
{
	while(env)
	{
		if (env->variable)
			free(env->variable);
		if (env->content)
			free(env->content);
		env = env->next;
	}
	free(env);
}

void	free_tokenizer(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->content)
			ft_free_2d(tokens->content);
		if (tokens->type)
			free(tokens->type);
		tokens = tokens->next;
	}
	free(tokens);
}

void	init_minishell(t_shell *shell)
{
	t_token		*tokens;
	t_parser	*parser;

	while (1)
	{
		shell->prompt = readline("Arab Spring 🐣🐥 -> ");
		if (shell->prompt)
		{
			add_history(shell->prompt);
			tokens = tokenizer(shell);
			parser = ft_calloc(1, sizeof(t_parser));
			if (!parser)
				exit(EXIT_FAILURE);
			ft_parser(tokens, parser, shell);
			// ft_expander(shell, tokens);
			if (tokens)
				execute(shell, tokens, parser);
			free(parser);
			if(shell->enviroment)
			{
				ft_free_2d(shell->enviroment);
				shell->enviroment = NULL;
			}
			if (tokens)
			{
				free_tokenizer(tokens);
				free(tokens);
				tokens = NULL;
			}
		}
		else
		{
			if (shell->env)
			{
				free_env(shell->env);
				// free(shell->env);
			}
			// if (tokens)
			// {
			// 	free_tokenizer(tokens);
			// 	free(tokens);
			// 	tokens = NULL;
			// }
			free(shell);
			exit(0);
		}
		if (tokens)
		{
			free_tokenizer(tokens);
			free (tokens);
			tokens= NULL;
		}
	}
}

// take the arguments and the enviroment
int	main(int argc, char **argv, char **env)
{
	t_shell		*shell;

	(void)argv;
	if (argc > 1)
	{
		printf("Error\nwrong number of arguments\n");
		return (1);
	}
	shell = ft_calloc(1, sizeof(t_shell));
	if (!shell)
		exit(EXIT_FAILURE);
	env_copy(shell, env);
	init_minishell(shell);
	if (shell->env)
	{
		free_env(shell->env);
		free(shell->env);
	}
	free(shell->prompt);
	free(shell);
	rl_clear_history();
	return (0);
}
