/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:48:48 by qhatahet          #+#    #+#             */
/*   Updated: 2025/05/09 14:08:02 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_pipes(t_token *tokens, t_shell *shell, t_parser *parser)
{
	int	i;
	t_fds	*fds;
	int	pipes[how_many_pipes(tokens)][2];
	int	pids[how_many_pipes(tokens) + 1];
	t_token	*temp;

	i = 0;
	temp = tokens;
	while (i < how_many_pipes(tokens))
	{
		if (pipe(pipes[i]) == -1) 
		{
			fprintf(stderr, "Error with creating pipe\n");
			return;
		}
		i++;
	}
	i = 0;
	while (i < how_many_pipes(tokens) + 1 && temp)
	{
		shell->cmd_list;
		pids[i] = fork();
	}
}
