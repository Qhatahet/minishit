/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 05:58:19 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/17 00:46:07 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	quote_norm(t_token *token, t_expand *expand, char *temp, int *count)
{
	while (token->content[expand->out][expand->in])
	{
		if (*count % 2 == 0)
		{
			while (token->content[expand->out][expand->in])
			{
				if (token->content[expand->out][expand->in] == '\''
					|| token->content[expand->out][expand->in] == '\"')
				{
					expand->quote = token->content[expand->out][expand->in];
					(*count)++;
					expand->in++;
					break ;
				}
				temp[expand->dex++] = token->content[expand->out][expand->in++];
			}
		}
		if (token->content[expand->out][expand->in] == expand->quote)
		{
			(*count)++;
			expand->in++;
		}
		else
			temp[expand->dex++] = token->content[expand->out][expand->in++];
	}
}

static void	process_quotes(t_token *token, t_expand *expand, char *temp)
{
	int	count;

	expand->dex = 0;
	count = 0;
	expand->in = 0;
	expand->quote = '\0';
	while (token->content[expand->out][expand->in])
	{
		if (token->content[expand->out][expand->in] == '\''
			|| token->content[expand->out][expand->in] == '\"')
		{
			expand->quote = token->content[expand->out][expand->in];
			count++;
			expand->in++;
			break ;
		}
		else
			temp[expand->dex++] = token->content[expand->out][expand->in++];
	}
	quote_norm(token, expand, temp, &count);
	temp[expand->dex] = '\0';
}

void	quote_remover(t_token *token, t_expand *expand)
{
	char	*temp;
	int		x;

	if (!token || !token->content)
		return ;
	x = count_quotes(token, expand);
	if (x == 0)
		return ;
	temp = malloc(ft_strlen(token->content[expand->out]) - x + 1);
	if (!temp)
		exit(123);
	process_quotes(token, expand, temp);
	free(token->content[expand->out]);
	token->content[expand->out] = temp;
}

static void	count_extension(t_token *token, t_expand *expand, int *count)
{
	while (token->content[expand->out][expand->in])
	{
		if (*count % 2 == 0)
		{
			while (token->content[expand->out][expand->in])
			{
				if (token->content[expand->out][expand->in] == '\''
					|| token->content[expand->out][expand->in] == '\"')
				{
					expand->quote = token->content[expand->out][expand->in];
					(*count)++;
					expand->in++;
					break ;
				}
				expand->in++;
			}
		}
		if (token->content[expand->out][expand->in] == expand->quote)
			(*count)++;
		expand->in++;
	}
}

// count quotes to be removed
int	count_quotes(t_token *token, t_expand *expand)
{
	int	count;

	count = 0;
	expand->in = 0;
	expand->quote = '\0';
	if (!token->content || !token->content[expand->out])
		return (0);
	while (token->content[expand->out][expand->in])
	{
		if (token->content[expand->out][expand->in] == '\''
			|| token->content[expand->out][expand->in] == '\"')
		{
			expand->quote = token->content[expand->out][expand->in];
			count++;
			expand->in++;
			break ;
		}
		expand->in++;
	}
	count_extension(token, expand, &count);
	return (count);
}
