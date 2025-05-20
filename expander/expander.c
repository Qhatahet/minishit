/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:06:40 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/17 00:48:18 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_token(t_shell *shell, t_token *token, t_expand *expand)
{
	if (!token || !token->content)
		return ;
	while (token->content[expand->out])
	{
		expand->in = 0;
		expand->single_qoute = 0;
		if (token->type[expand->out] == TEXT)
			expand_dollar(shell, token, expand);
		if (token->type[expand->out] == TEXT
			|| token->type[expand->out] == FILENAME)
			quote_remover(token, expand);
		expand->out++;
	}
	if (expand->variable)
	{
		free(expand->variable);
		expand->variable = NULL;
	}
}

void	ft_expander(t_shell *shell, t_token *token)
{
	t_expand	*expand;

	expand = ft_calloc(1, sizeof(t_expand));
	if (!expand)
		return ;
	if (!token)
		return ;
	while (token)
	{
		expand->out = 0;
		process_token(shell, token, expand);
		token = token->next;
	}
	if (expand->variable)
		free(expand->variable);
	free(expand);
}

void	check_cmd(t_token *token, t_expand *expand, char **paths)
{
	char	*command_path;
	int		i;

	command_path = NULL;
	i = 0;
	while (paths[i])
	{
		command_path = ft_strjoin(paths[i], token->content[expand->out]);
		if (!command_path)
			exit(EXIT_FAILURE);
		if (!access(command_path, X_OK))
		{
			token->type[expand->out] = COMMAND;
			break ;
		}
		i++;
	}
	if (command_path)
		free(command_path);
}
