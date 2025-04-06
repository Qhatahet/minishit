/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 02:45:52 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/01 21:31:00 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_arument(t_shell *shell, t_token *token)
{
	int	i;

	shell->temp_index = 0;
	shell->echo_flag = 0;
	while (token->content[shell->temp_index])
	{
		if (token->type[shell->temp_index] == ARGUMENT)
		{
			i = 1;
			while (token->content[shell->temp_index][i])
			{
				if (token->content[shell->temp_index][i] != 'n')
				{
					token->type[shell->temp_index] = TEXT;
					shell->echo_flag = 0;
					break ;
				}
				i++;
				shell->echo_flag = 1;
			}
		}
		shell->temp_index++;
	}
}

void	echo_command(t_shell *shell, t_token *token)
{
	echo_arument(shell, token);
	shell->temp_index = 1;
	while (token->content[shell->temp_index])
	{
		if (token->type[shell->temp_index] != ARGUMENT)
		{
			printf("%s", token->content[shell->temp_index]);
			if (token->content[shell->temp_index + 1]
				&& token->type[shell->temp_index + 1] != ARGUMENT)
				printf(" ");
		}
		shell->temp_index++;
	}
	if (shell->echo_flag != 1)
		printf("\n");
}
