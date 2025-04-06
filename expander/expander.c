/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:06:40 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/04 15:44:08 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_quotes(t_token *token, t_expand *expand)
{
	while (token->content[expand->outer][expand->inner])
	{
		if (token->content[expand->outer][expand->inner] == '\"'
			|| token->content[expand->outer][expand->inner] == '\'')
			quote_remover(token, expand);
		if (expand->quote_flag)
			break ;
		expand->inner++;
	}
}

static void	process_dollar(t_shell *shell, t_token *token, t_expand *expand)
{
	while (token->content[expand->outer][expand->inner])
	{
		if (!expand->single_qoute
			&& token->content[expand->outer][expand->inner] == '$')
			{
				expand_dollar(shell, token, expand);
				expand->inner = 0;
			}
		expand->inner++;
	}
}

static void	process_token(t_shell *shell, t_token *token, t_expand *expand)
{
	while (token->content[expand->outer])
	{
		expand->inner = 0;
		expand->quote_flag = 0;
		expand->single_qoute = 0;
		if (token->type[expand->outer] == TEXT)
		{
			process_quotes(token, expand);
			if (token->content[expand->outer][0] != '\0')
				check_cmd(token, expand, shell->paths);
			expand->inner = 0;
			process_dollar(shell, token, expand);
		}
		expand->outer++;
	}
}

void	ft_expander(t_shell *shell, t_token *token)
{
	t_expand	*expand;

	expand = ft_calloc(1, sizeof(t_expand));
	if (!expand)
		return ;
	expand->inner = 0;
	expand->quote_flag = 0;
	expand->single_qoute = 0;
	while (token)
	{
		expand->outer = 0;
		process_token(shell, token, expand);
		token = token->next;
	}
	free(expand);
}
