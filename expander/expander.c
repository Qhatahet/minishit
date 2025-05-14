/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:06:40 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/14 01:58:43 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_token(t_shell *shell, t_token *token, t_expand *expand)
{
	while (token->content[expand->outer])
	{
		expand->inner = 0;
		expand->single_qoute = 0;
		if (token->type[expand->outer] == TEXT)
			expand_dollar(shell, token, expand);
		if(token->type[expand->outer] == TEXT
			|| token->type[expand->outer] == FILENAME)
			quote_remover(token, expand);
		expand->outer++;
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
	while (token)
	{
		expand->outer = 0;
		process_token(shell, token, expand);
		token = token->next;
	}
	if (expand->variable)
		free(expand->variable);
	free(expand);
}
