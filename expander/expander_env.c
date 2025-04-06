/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:10:23 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/05 15:47:44 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_length(t_token *token, t_expand *expand)
{
	while ((ft_isalpha(token->content[expand->outer][expand->inner])
		|| token->content[expand->outer][expand->inner] == '_')
		&& token->content[expand->outer][expand->inner])
	{
		expand->var_length++;
		expand->inner++;
	}
	expand->inner -= expand->var_length;
	// printf("ls %d\n",expand->var_length);
	// printf("ex %d\n",expand->inner);
	expand->var_length++;
}

void	copy_var(t_token *token, t_expand *expand)
{
	int	i;

	get_length(token, expand);
	expand->variable = malloc(expand->var_length + 1);
	if (!expand->variable)
		return ;
	i = 0;
	while (token->content[expand->outer][expand->inner]
		&& (ft_isalpha(token->content[expand->outer][expand->inner])
		|| token->content[expand->outer][expand->inner] == '_'))
	{
		expand->variable[i] = token->content[expand->outer][expand->inner];
		i++;
		expand->inner++;
	}
	expand->variable[i] = '\0';
	expand->inner -= i;
}

void	check_env(t_shell *shell, t_expand *expand)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->variable, expand->variable) == 0)
		{
			expand->output = ft_strdup(current->content);
			return ;
		}
		current = current->next;
	}
	expand->output = "\0";
}
