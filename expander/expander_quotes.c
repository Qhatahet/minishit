/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 05:58:19 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/26 18:36:15 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quote_remover(t_token *token, t_expand *expand)
{
	char	*temp;
	int		i;
	int count;

	int x = count_quotes(token, expand);
	if(x == 0)
		return;
	temp = malloc(ft_strlen(token->content[expand->outer]) - x + 1);
	if(!temp)
		exit(123);
	expand->inner = 0;
	i = 0;
	count = 0;
	expand->quote = '\0';
	while(token->content[expand->outer][expand->inner])
	{
		if(token->content[expand->outer][expand->inner] == '\'' || token->content[expand->outer][expand->inner] == '\"')
		{
			expand->quote = token->content[expand->outer][expand->inner];
			count++;
			expand->inner++;
			break ;
		}
		else
			temp[i++] = token->content[expand->outer][expand->inner++];
	}
	while (token->content[expand->outer][expand->inner])
	{
		if(count % 2 == 0)
		{
			while(token->content[expand->outer][expand->inner])
			{
				if(token->content[expand->outer][expand->inner] == '\'' || token->content[expand->outer][expand->inner] == '\"')
				{
					expand->quote = token->content[expand->outer][expand->inner];
					count++;
					expand->inner++;
					break ;
				}
				temp[i++] = token->content[expand->outer][expand->inner++];
			}
		}
		if (token->content[expand->outer][expand->inner] == expand->quote)
		{
			count++;
			expand->inner++;
		}
		else
			temp[i++] = token->content[expand->outer][expand->inner++];
	}
	temp[i] = '\0';
	free(token->content[expand->outer]);
	token->content[expand->outer] = temp;
}

// count quotes to be removed 
int	count_quotes(t_token *token, t_expand *expand)
{
	int count;
	
	count = 0;
	expand->inner = 0;
	while(token->content[expand->outer][expand->inner])
	{
		if(token->content[expand->outer][expand->inner] == '\'' || token->content[expand->outer][expand->inner] == '\"')
		{
			expand->quote = token->content[expand->outer][expand->inner];
			count++;
			expand->inner++;
			break ;
		}
		expand->inner++;
	}
	while (token->content[expand->outer][expand->inner])
	{
		if(count % 2 == 0)
		{
			while(token->content[expand->outer][expand->inner])
			{
				if(token->content[expand->outer][expand->inner] == '\'' || token->content[expand->outer][expand->inner] == '\"')
				{
					expand->quote = token->content[expand->outer][expand->inner];
					count++;
					expand->inner++;
					break ;
				}
				expand->inner++;
			}
		}
		if (token->content[expand->outer][expand->inner] == expand->quote)
			count++;
		expand->inner++;
	}
	return(count);
}
