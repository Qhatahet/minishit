/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:01:02 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/22 14:08:28 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	count_without_qoutes(t_shell *shell)
{
	while (shell->temp_index < (int)ft_strlen(shell->prompt)
		&& shell->prompt[shell->temp_index] != '>'
		&& shell->prompt[shell->temp_index] != '|'
		&& shell->prompt[shell->temp_index] != ' '
		&& shell->prompt[shell->temp_index] != '<')
	{
		// && shell->prompt[shell->temp_index]
		if (shell->prompt[shell->temp_index] == '\'')
		{
			shell->temp_index++;
			while (shell->prompt[shell->temp_index] != '\''
				&& shell->prompt[shell->temp_index])
				shell->temp_index++;
		}
		else if (shell->prompt[shell->temp_index] == '\"')
		{
			shell->temp_index++;
			while (shell->prompt[shell->temp_index] != '\"'
				&& shell->prompt[shell->temp_index])
				shell->temp_index++;
		}
		shell->temp_index++;
	}
}

void	count_redirection(t_shell *shell)
{
	if (shell->prompt[shell->temp_index] == '<')
	{
		shell->temp_index++;
		if (shell->prompt[shell->temp_index] == '<')
			shell->temp_index++;
	}
	else if (shell->prompt[shell->temp_index] == '>')
	{
		shell->temp_index++;
		if (shell->prompt[shell->temp_index] == '>')
			shell->temp_index++;
	}
	shell->counter++;
}

void	tokenizer_size(t_shell *shell)
{
	while (shell->temp_index < (int)ft_strlen(shell->prompt)
		&& shell->prompt[shell->temp_index] != '\0'
		&& shell->prompt[shell->temp_index] != '|')
	{
		if (shell->prompt[shell->temp_index] == ' '
			|| shell->prompt[shell->temp_index] == '\t')
			shell->temp_index++;
		else if (shell->prompt[shell->temp_index] == '<')
			count_redirection(shell);
		else if (shell->prompt[shell->temp_index] == '>')
			count_redirection(shell);
		else if (shell->prompt[shell->temp_index] != '\0')
		{
			count_without_qoutes(shell);
			shell->counter++;
		}
	}
	// printf("counter = %d\n", shell->counter);
}
