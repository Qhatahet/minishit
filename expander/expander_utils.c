/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 05:58:19 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/05 02:40:38 by oalananz         ###   ########.fr       */
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

void	quote_remover(t_token *token, t_expand *expand)
{
	char	*temp;
	int		i;

	expand->quote_flag = 1;
	count_quotes(token, expand);
	if (expand->quote_flag == 2)
		return ;
	expand->inner = 0;
	i = 0;
	temp = malloc(ft_strlen(token->content[expand->outer])
			- expand->quotes_count + 1);
	if (!temp)
		return ;
	while (token->content[expand->outer][expand->inner])
	{
		if (token->content[expand->outer][expand->inner] == expand->quote)
			expand->inner++;
		else
			temp[i++] = token->content[expand->outer][expand->inner++];
	}
	temp[i] = '\0';
	free(token->content[expand->outer]);
	token->content[expand->outer] = temp;
}

void	expand_dollar(t_shell *shell, t_token *token, t_expand *expand)
{
	expand->inner++;
	if (ft_isalpha(token->content[expand->outer][expand->inner]) || token->content[expand->outer][expand->inner] == '_')
	{
		copy_var(token, expand);
		check_env(shell, expand);
	}
	if (ft_isdigit(token->content[expand->outer][expand->inner])
		&& token->content[expand->outer][expand->inner] == '0')
		expand->output = ft_strdup("bash");
	ft_outjoin(token, expand);
}

void	count_quotes(t_token *token, t_expand *expand)
{
	expand->quotes_count = 0;
	expand->quote = token->content[expand->outer][expand->inner];
	if (expand->quote == '\'')
		expand->single_qoute = 1;
	expand->quotes_count++;
	expand->inner++;
	while (token->content[expand->outer][expand->inner])
	{
		if (token->content[expand->outer][expand->inner] == expand->quote)
			expand->quotes_count++;
		expand->inner++;
	}
	if (expand->quotes_count % 2 != 0)
		expand->quote_flag = 2;
}
