/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:10:23 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/24 20:12:04 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	check_cmd(t_token *token, t_expand *expand, char **paths)
{
	char	*command_path;
	int		i;

	command_path = NULL;
	i = 0;
	while (paths[i])
	{
		command_path = ft_strjoin(paths[i], token->content[expand->outer]);
		if (!command_path)
			exit(EXIT_FAILURE);
		if (!access(command_path, X_OK))
		{
			token->type[expand->outer] = COMMAND;
			break ;
		}
		i++;
	}
	if (command_path)
		free(command_path);
}