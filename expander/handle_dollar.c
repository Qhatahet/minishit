/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 01:27:50 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/17 01:33:57 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	numlen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

int	count_dollar(t_token *token, t_expand *expand)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (token->content[expand->out][i])
	{
		if (token->content[expand->out][i] == '$')
			count++;
		i++;
	}
	return (count);
}

static void	handle_2(t_shell *shell, t_token *token, t_expand *expand,
		char *temp)
{
	int		count;
	t_env	*current;

	count = 0;
	while (token->content[expand->out][expand->in]
		&& (ft_isalnum(token->content[expand->out][expand->in])
			|| token->content[expand->out][expand->in] == '_'))
	{
		expand->in++;
		count++;
	}
	expand->variable = ft_substr(token->content[expand->out], expand->in
			- count, count);
	count = 0;
	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->variable, expand->variable) == 0)
			while (current->content[count])
				temp[expand->dex++] = current->content[count++];
		current = current->next;
	}
	free(expand->variable);
	expand->variable = NULL;
}

static void	handle_1(t_shell *shell, t_token *token, t_expand *expand,
		char *temp)
{
	char	*digit;
	int		count;

	count = 0;
	if (ft_isdigit(token->content[expand->out][expand->in])
		&& token->content[expand->out][expand->in] == '0')
	{
		digit = ft_strdup("ARSSH");
		expand->in++;
		while (digit[count])
			temp[expand->dex++] = digit[count++];
		free(digit);
	}
	else if (ft_isdigit(token->content[expand->out][expand->in]))
		expand->in++;
	else
		handle_2(shell, token, expand, temp);
}

void	handle_dollar(t_shell *shell, t_token *token, t_expand *expand,
		char *temp)
{
	char	*tmp;
	int		count;

	expand->in++;
	if (token->content[expand->out][expand->in] == ' '
		|| token->content[expand->out][expand->in] == '\0')
		temp[expand->dex++] = '$';
	else if (token->content[expand->out][expand->in] == '?')
	{
		count = 0;
		tmp = ft_itoa(shell->exit_status);
		expand->in++;
		while (tmp[count])
			temp[expand->dex++] = tmp[count++];
		free(tmp);
	}
	else
		handle_1(shell, token, expand, temp);
	if (expand->variable)
	{
		free(expand->variable);
		expand->variable = NULL;
	}
}
