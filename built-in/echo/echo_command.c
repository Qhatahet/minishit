/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 02:45:52 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/05 17:57:35 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_arument(t_shell *shell, t_token *token)
{
	int	i;

	shell->echo_flag = 0;
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
		shell->temp_index++;
	}
}

void	echo_command(t_shell *shell, t_token *token)
{
	shell->temp_index = 1;
	echo_arument(shell, token);
	while (token->content[shell->temp_index])
	{
		if (token->content[shell->temp_index])
		{
			printf("%s", token->content[shell->temp_index]);
			if (token->content[shell->temp_index + 1])
				printf(" ");
		}
		shell->temp_index++;
	}
	if (shell->echo_flag != 1)
		printf("\n");
}
