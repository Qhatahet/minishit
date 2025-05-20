/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:10:23 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/17 01:43:28 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	content_length(t_token *token, t_expand *expand, t_shell *shell)
{
	t_env	*current;
	int		count;

	count = 0;
	while (token->content[expand->out][expand->in]
		&& (ft_isalpha(token->content[expand->out][expand->in])
			|| token->content[expand->out][expand->in] == '_'))
	{
		expand->in++;
		count++;
	}
	expand->variable = ft_substr(token->content[expand->out], expand->in
			- count, count);
	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->variable, expand->variable) == 0)
		{
			count = ft_strlen(current->content);
			return (count);
		}
		current = current->next;
	}
	return (count);
}

static int	content_len(t_shell *shell, t_token *token, t_expand *expand)
{
	int	count;

	count = 0;
	expand->in++;
	if (token->content[expand->out][expand->in] == ' '
		|| token->content[expand->out][expand->in] == '\0')
		count++;
	else if (token->content[expand->out][expand->in] == '?')
	{
		expand->in++;
		count += numlen(shell->exit_status);
	}
	else if (token->content[expand->out][expand->in] == '0')
	{
		expand->in++;
		count += 5;
	}
	else if (ft_isdigit(token->content[expand->out][expand->in]))
		expand->in++;
	else
		count += content_length(token, expand, shell);
	return (count);
}

static void	count_norm(t_token *token, t_expand *expand, t_shell *shell,
		int *size)
{
	if (token->content[expand->out][expand->in] == '\'')
	{
		(*size)++;
		expand->in++;
		while (token->content[expand->out][expand->in]
			&& token->content[expand->out][expand->in] != '\'')
		{
			expand->in++;
			(*size)++;
		}
		(*size)++;
		expand->in++;
	}
	else if (token->content[expand->out][expand->in] == '$')
	{
		(*size) += content_len(shell, token, expand);
		free(expand->variable);
		expand->variable = NULL;
	}
	else
	{
		expand->in++;
		(*size)++;
	}
}

static void	double_quote(t_token *token, t_expand *expand, t_shell *shell,
		int *size)
{
	if (token->content[expand->out][expand->in] == '\"')
	{
		(*size)++;
		expand->in++;
		while (token->content[expand->out][expand->in]
			&& token->content[expand->out][expand->in] != '\"')
		{
			if (token->content[expand->out][expand->in] == '$')
			{
				(*size) += content_len(shell, token, expand) - 1;
				free(expand->variable);
				expand->variable = NULL;
			}
			expand->in++;
			(*size)++;
		}
	}
}

int	count_length(t_shell *shell, t_token *token, t_expand *expand)
{
	int	size;

	expand->in = 0;
	size = 0;
	if (!token->content || !token->content[expand->out])
		return (0);
	while (expand->in < (int)ft_strlen(token->content[expand->out])
		&& token->content[expand->out][expand->in])
	{
		double_quote(token, expand, shell, &size);
		count_norm(token, expand, shell, &size);
	}
	expand->in = 0;
	return (size);
}
