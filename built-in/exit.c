/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:34:44 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/16 11:54:20 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void free_str_array(char **arr)
{
	int i = 0;

	if (arr)
	{
		while (arr[i])
		{
			if (arr[i])
				free(arr[i]);
			i++;
		}
		free(arr);
	}
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
		if (tokens->heredoc_file)
		{
			unlink(tokens->heredoc_file);
			free(tokens->heredoc_file);
		}
		free(tokens);
		tokens = temp;
	}
	free(tokens);
}

void free_shell(t_shell *shell)
{
	if (!shell)
		return;
	if (shell->cmd_list)
		free_str_array(shell->cmd_list);
	if (shell->paths)
		free_str_array(shell->paths);
	if (shell->prompt)
		free(shell->prompt);
	if (shell->enviroment)
		free_str_array(shell->enviroment);
	if (shell->variable)
		free(shell->variable);
	if (shell->env)
		free_env(shell->env);
	free(shell);
}

int valid_arg(char *status)
{
    int i = 0;
    while(status[i])
    {
        if(ft_isdigit(status[i]))
            i++;
        else
            return(-1);
    }
    return(1);
}

void    ft_exit(t_token *token , t_shell *shell)
{
    if(token->content[1])
    {
        if(valid_arg(token->content[1]) == -1)
        {
            char *temp = ft_strdup("arssh:  exit: ");
            char *tmp = ft_strjoin(temp,token->content[1]);
            free(temp);
            temp = ft_strjoin(tmp,": numeric argument required\n");
            free(tmp);
            write(2,temp,ft_strlen(temp));
            free(temp);
            free_shell(shell);
            free_tokenizer(token);
            exit(2);
        }
        int status = ft_atoi(token->content[1]);
        free_shell(shell);
        free_tokenizer(token);
        exit(status);
    }
    else
    {
        free_shell(shell);
        free_tokenizer(token);
        exit(0);
    }
}
