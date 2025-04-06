/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 13:59:09 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/26 16:28:18 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes_norm(t_shell *shell, int *quote, int i, char c)
{
	(*quote) = 1;
	i++;
	while (shell->prompt[i] != c && shell->prompt[i])
		i++;
	if (shell->prompt[i] == c && shell->prompt[i])
	{
		(*quote) = 2;
		i++;
	}
	return (i);
}

void	check_quotes(t_shell *shell)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	while (shell->prompt[i])
	{
		if (shell->prompt[i] == '\"' && shell->prompt[i])
			i = check_quotes_norm(shell, &quote, i, '\"');
		else if (shell->prompt[i] == '\'' && shell->prompt[i])
			i = check_quotes_norm(shell, &quote, i, '\'');
		else
			i++;
	}
	if (quote % 2 != 0)
	{
		printf("arssh: unclosed qoutes\n");
		shell->token_flag = 1;
	}
}

int	check_pipe(t_shell *shell)
{
	int	i;

	i = shell->prompt_index;
	if (shell->prompt[i] == '|')
		i++;
	if (shell->prompt[i] == '\0')
		return (-1);
	while (shell->prompt[i] && shell->prompt[i] == ' ')
		i++;
	if (shell->prompt[i] == '\0')
		return (-1);
	return (1);
}

t_token	*tokenizer(t_shell *shell)
{
	t_token	*head;
	t_token	*token;

	shell->prompt_index = 0;
	shell->token_index = 0;
	shell->token_flag = 0;
	shell->counter = 0;
	head = create_new_node(shell);
	token = head;
	while (shell->prompt[shell->prompt_index] == ' ')
		shell->prompt_index++;
	while (shell->prompt_index < (int)ft_strlen(shell->prompt)
		&& shell->prompt[shell->prompt_index])
	{
		check_errors(shell);
		if (shell->token_flag)
			return (NULL);
		check_prompt_2(shell, &token);
		check_prompt_3(shell, &token);
		while (shell->prompt_index < (int)ft_strlen(shell->prompt)
			&& shell->prompt[shell->prompt_index] == ' ')
			shell->prompt_index++;
	}
	token->content[shell->token_index] = NULL;
	return (head);
}
