/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 01:41:38 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/17 01:42:23 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	double_quote_handle(t_token *token, t_expand *expand,
		t_shell *shell, char *temp)
{
	if (token->content[expand->out][expand->in] == '\"')
	{
		temp[expand->dex++] = token->content[expand->out][expand->in++];
		while (token->content[expand->out][expand->in]
			&& token->content[expand->out][expand->in] != '\"')
		{
			if (token->content[expand->out][expand->in] == '$')
			{
				handle_dollar(shell, token, expand, temp);
				if (expand->variable)
				{
					free(expand->variable);
					expand->variable = NULL;
				}
			}
			else
				temp[expand->dex++] = token->content[expand->out][expand->in++];
		}
	}
}

static void	single_quote_handle(t_token *token, t_expand *expand,
		t_shell *shell, char *temp)
{
	if (token->content[expand->out][expand->in] == '\'')
	{
		temp[expand->dex++] = token->content[expand->out][expand->in++];
		while (token->content[expand->out][expand->in]
			&& token->content[expand->out][expand->in] != '\'')
			temp[expand->dex++] = token->content[expand->out][expand->in++];
		temp[expand->dex++] = token->content[expand->out][expand->in++];
	}
	else if (token->content[expand->out][expand->in] == '$')
	{
		handle_dollar(shell, token, expand, temp);
		if (expand->variable)
		{
			free(expand->variable);
			expand->variable = NULL;
		}
	}
	else if (token->content[expand->out][expand->in])
		temp[expand->dex++] = token->content[expand->out][expand->in++];
}

void	expand_dollar(t_shell *shell, t_token *token, t_expand *expand)
{
	char	*temp;
	int		x;

	expand->dex = 0;
	x = count_length(shell, token, expand);
	if ((int)ft_strlen(token->content[expand->out]) == x)
		return ;
	temp = malloc(x + count_dollar(token, expand));
	if (!temp)
		exit(130);
	expand->variable = NULL;
	while (token->content[expand->out][expand->in])
	{
		double_quote_handle(token, expand, shell, temp);
		single_quote_handle(token, expand, shell, temp);
	}
	temp[expand->dex] = '\0';
	free(token->content[expand->out]);
	token->content[expand->out] = temp;
}
