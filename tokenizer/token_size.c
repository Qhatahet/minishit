/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:01:02 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/01 04:30:00 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	count_qoutes(t_shell *shell)
{
	char	detect;

	detect = shell->prompt[shell->temp_index];
	shell->temp_index++;
	while (shell->prompt[shell->temp_index] != detect)
		shell->temp_index++;
	while (shell->prompt[shell->temp_index])
	{
		if (shell->prompt[shell->temp_index] == detect)
			shell->qoute_flag = 1;
		if (shell->qoute_flag == 1 && (shell->prompt[shell->temp_index] == ' '
				|| shell->prompt[shell->temp_index] == '|'
				|| shell->prompt[shell->temp_index] == '>'
				|| shell->prompt[shell->temp_index] == '<'))
			break ;
		shell->temp_index++;
	}
}

void	count_without_qoutes(t_shell *shell)
{
	while (shell->prompt[shell->temp_index] != '<'
		&& shell->prompt[shell->temp_index] != '>'
		&& shell->prompt[shell->temp_index] != '|'
		&& shell->prompt[shell->temp_index] != ' '
		&& shell->prompt[shell->temp_index])
	{
		if (shell->prompt[shell->temp_index] == '\'')
		{
			shell->temp_index++;
			while (shell->prompt[shell->temp_index] != '\'')
				shell->temp_index++;
		}
		else if (shell->prompt[shell->temp_index] == '\"')
		{
			shell->temp_index++;
			while (shell->prompt[shell->temp_index] != '\"')
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
			if (shell->prompt[shell->temp_index] != '\"'
				&& shell->prompt[shell->temp_index] != '\''
				&& shell->prompt[shell->temp_index])
				count_without_qoutes(shell);
			if (shell->prompt[shell->temp_index] == '\"'
				|| shell->prompt[shell->temp_index] == '\'')
				count_qoutes(shell);
			shell->counter++;
		}
	}
	printf("counter = %d\n", shell->counter);
}
