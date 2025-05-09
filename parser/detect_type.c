/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_type.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:16:39 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/26 19:14:48 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_built_ins(t_parser *parser, t_token *temp)
{
	int	i;

	i = 0;
	if (!ft_strcmp(temp->content[parser->index], "cd"))
		i = 1;
	else if (!ft_strcmp(temp->content[parser->index], "export"))
		i = 1;
	else if (!ft_strcmp(temp->content[parser->index], "unset"))
		i = 1;
	else if (!ft_strcmp(temp->content[parser->index], "exit"))
		i = 1;
	else
		i = 0;
	if (i == 1)
	{
		temp->type[parser->index] = COMMAND;
		parser->command_flag = 1;
		return (1);
	}
	else
		return (0);
}

void	detect_command(t_parser *parser, t_token *temp, char **paths)
{
	char	*command_path;
	int		i;

	command_path = NULL;
	i = 0;
	if (check_built_ins(parser, temp) == 1)
		return ;
	while (paths[i])
	{
		command_path = ft_strjoin(paths[i], temp->content[parser->index]);
		if (!command_path)
			exit(EXIT_FAILURE);
		if (!access(command_path, X_OK))
		{
			temp->type[parser->index] = COMMAND;
			parser->command_flag = 1;
			if (command_path)
				free(command_path);
			break ;
		}
		i++;
		free(command_path);
	}
}

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
