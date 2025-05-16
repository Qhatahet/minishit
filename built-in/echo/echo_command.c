/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 02:45:52 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/14 19:23:33 by oalananz         ###   ########.fr       */
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
		while (shell->cmd_list[shell->temp_index][i])
		{
			if (shell->cmd_list[shell->temp_index][i] != 'n')
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
	while (shell->cmd_list[shell->temp_index])
	{
		if (shell->cmd_list[shell->temp_index])
		{
			write(shell->fd_out,shell->cmd_list[shell->temp_index],ft_strlen(shell->cmd_list[shell->temp_index]));
			if (shell->cmd_list[shell->temp_index + 1])
				write(shell->fd_out," ",1);
		}
		shell->temp_index++;
	}
	if (shell->echo_flag != 1)
		write(shell->fd_out,"\n",1);
}
