/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_type.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:16:39 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/17 01:46:54 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	detect_arguments(t_parser *parser, t_token *temp)
{
	if (temp->content[parser->index][0] == '-'
		&& ft_isalpha(temp->content[parser->index][1]))
	{
		temp->type[parser->index] = ARGUMENT;
	}
}

void	detect_heredoc(t_parser *parser, t_token *temp)
{
	if (ft_strncmp(temp->content[parser->index], "<<", 2) == 0)
	{
		temp->type[parser->index] = HEREDOC;
		parser->eof_flag = 1;
	}
	else if (ft_strncmp(temp->content[parser->index], ">>", 2) == 0)
	{
		temp->type[parser->index] = APPEND;
		parser->filename_flag = 1;
	}
}

void	detect_redirect(t_parser *parser, t_token *temp)
{
	if (*temp->content[parser->index] == '>')
	{
		temp->type[parser->index] = REDIRECTOUT;
		parser->filename_flag = 1;
	}
	else if (*temp->content[parser->index] == '<')
	{
		temp->type[parser->index] = REDIRECTIN;
		parser->filename_flag = 1;
	}
}

int	detect_filename(t_parser *parser, t_token *temp)
{
	if (parser->eof_flag && temp->content[parser->index])
	{
		temp->type[parser->index] = ENDOFFILE;
		return (1);
	}
	else if (parser->filename_flag && temp->content[parser->index])
	{
		temp->type[parser->index] = FILENAME;
		return (1);
	}
	return (0);
}
