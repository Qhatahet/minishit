/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:10:34 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/26 16:27:06 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_prompt_4(t_shell *shell)
{
	if (ft_strncmp(&shell->prompt[shell->prompt_index], "||||", 4) == 0)
	{
		printf("arssh: syntax error near unexpected token `||'\n");
		shell->token_flag = 1;
		shell->prompt_index += 4;
	}
	else if (ft_strncmp(&shell->prompt[shell->prompt_index], "|||", 3) == 0)
	{
		printf("arssh: syntax error near unexpected token `|'\n");
		shell->token_flag = 1;
		shell->prompt_index += 3;
	}
	else if (ft_strncmp(&shell->prompt[shell->prompt_index], "||", 2) == 0)
	{
		printf("arssh: this shell doesn't support the or operator `||'\n");
		shell->token_flag = 1;
		shell->prompt_index += 2;
	}
	else if (ft_strncmp(&shell->prompt[shell->prompt_index], "|", 1) == 0
		&& check_pipe(shell) < 0)
	{
		printf("arssh: unclosed pipe\n");
		shell->token_flag = 1;
		shell->prompt_index += 1;
	}
}

void	check_prompt_1(t_shell *shell)
{
	if (ft_strncmp(&shell->prompt[shell->prompt_index], "<<<", 3) == 0)
	{
		printf("arssh: syntax error near unexpected token `<<'\n");
		shell->token_flag = 1;
		shell->prompt_index += 3;
	}
	else if (ft_strncmp(&shell->prompt[shell->prompt_index], ">>>>", 4) == 0)
	{
		printf("arssh: syntax error near unexpected token `>>'\n");
		shell->token_flag = 1;
		shell->prompt_index += 4;
	}
	else if (ft_strncmp(&shell->prompt[shell->prompt_index], ">>>", 3) == 0)
	{
		printf("arssh: syntax error near unexpected token `>'\n");
		shell->token_flag = 1;
		shell->prompt_index += 3;
	}
	else if (ft_strncmp(&shell->prompt[shell->prompt_index], "><", 2) == 0)
	{
		printf("arssh: syntax error near unexpected token `<'\n");
		shell->token_flag = 1;
		shell->prompt_index += 2;
	}
}

void	check_prompt_2(t_shell *shell, t_token **token)
{
	if (ft_strncmp(&shell->prompt[shell->prompt_index], "<<", 2) == 0)
	{
		(*token)->content[shell->token_index++] = ft_strdup("<<");
		shell->prompt_index += 2;
	}
	if (ft_strncmp(&shell->prompt[shell->prompt_index], ">>", 2) == 0)
	{
		(*token)->content[shell->token_index++] = ft_strdup(">>");
		shell->prompt_index += 2;
	}
	if (shell->prompt[shell->prompt_index] == '<')
	{
		(*token)->content[shell->token_index++] = ft_strdup("<");
		shell->prompt_index++;
	}
	if (shell->prompt[shell->prompt_index] == '>')
	{
		(*token)->content[shell->token_index++] = ft_strdup(">");
		shell->prompt_index++;
	}
	while (shell->prompt[shell->prompt_index] == ' ')
		shell->prompt_index++;
}

void	check_prompt_3(t_shell *shell, t_token **token)
{
	if (shell->prompt[shell->prompt_index] == '|')
	{
		(*token)->content[shell->token_index] = NULL;
		shell->prompt_index++;
		(*token)->next = create_new_node(shell);
		*token = (*token)->next;
		shell->token_index = 0;
	}
	else if (shell->prompt[shell->prompt_index]
		|| (shell->prompt[shell->prompt_index] == '-'
			&& ft_isalpha(shell->prompt[shell->prompt_index + 1])))
		copy(shell, *token);
	else if (shell->prompt[shell->prompt_index] == '\"')
		copy_quotes(shell, *token, '\"');
	else if (shell->prompt[shell->prompt_index] == '\'')
		copy_quotes(shell, *token, '\'');
	else
		shell->prompt_index++;
}

void	check_errors(t_shell *shell)
{
	check_quotes(shell);
	check_prompt_1(shell);
	check_prompt_4(shell);
}
