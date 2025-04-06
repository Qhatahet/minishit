/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_outjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 06:02:02 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/05 02:44:51 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_dollar(t_token *token, t_expand *expand, char *temp, int *i)
{
	int	j;

	j = 0;
	expand->inner++;
	if (ft_isalpha(token->content[expand->outer][expand->inner])
		|| token->content[expand->outer][expand->inner] == '_')
		expand->inner += expand->var_length - 1;
	else if (ft_isdigit(token->content[expand->outer][expand->inner]))
		expand->inner++;
	if (expand->output)
	{
		while (expand->output[j])
		{
			temp[(*i)] = expand->output[j];
			(*i)++;
			j++;
		}
		(*i)++;
	}
}

void	ft_outjoin(t_token *token, t_expand *expand)
{
	char	*temp;
	int		i;

	temp = ft_calloc(ft_strlen(token->content[expand->outer]) + ft_strlen(expand->output) - expand->var_length + ft_strlen(expand->output) , sizeof(char));
	if (!temp)
		return ;
	expand->inner = 0;
	i = 0;
	while (token->content[expand->outer][expand->inner])
	{
		if (token->content[expand->outer][expand->inner] == '$')
			handle_dollar(token, expand, temp, &i);
		temp[i] = token->content[expand->outer][expand->inner];
		i++;
		expand->inner++;
	}
	temp[i] = '\0';
	free(token->content[expand->outer]);
	token->content[expand->outer] = temp;
}
