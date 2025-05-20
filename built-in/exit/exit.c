/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:34:44 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/20 18:09:53 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_zero(t_token *token, t_shell *shell)
{
	g_exit_status = shell->exit_status;
	free_shell(shell);
	free_tokenizer(token);
	exit(g_exit_status);
}

void	ft_exit(t_token *token, t_shell *shell)
{
	char	*tmp;
	char	*temp;
	int		status;

	if (token->content[1])
	{
		if (valid_arg(token->content[1]) == -1)
		{
			tmp = ft_strjoin("ARSSH:  exit: ", token->content[1]);
			temp = ft_strjoin(tmp, ": numeric argument required\n");
			free(tmp);
			write(2, temp, ft_strlen(temp));
			free(temp);
			free_shell(shell);
			free_tokenizer(token);
			exit(2);
		}
		status = ft_atoi(token->content[1]);
		free_shell(shell);
		free_tokenizer(token);
		exit(status);
	}
	else
		exit_zero(token, shell);
}
