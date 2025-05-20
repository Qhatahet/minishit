/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:04:10 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/17 01:47:19 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_built_ins(t_parser *parser, t_token *temp)
{
	int	flag;

	flag = 0;
	if (!ft_strcmp(temp->content[parser->index], "cd"))
		flag = 1;
	else if (!ft_strcmp(temp->content[parser->index], "export"))
		flag = 1;
	else if (!ft_strcmp(temp->content[parser->index], "unset"))
		flag = 1;
	else if (!ft_strcmp(temp->content[parser->index], "exit"))
		flag = 1;
	else
		flag = 0;
	if (flag == 1)
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

void	add_backslash(t_shell *shell)
{
	int		i;
	int		j;
	char	*temp;
	int		l;

	i = 0;
	j = 0;
	temp = NULL;
	while (shell->paths && shell->paths[i])
	{
		l = ft_strlen(shell->paths[i]);
		temp = malloc(l + 2);
		while (shell->paths[i][j])
		{
			temp[j] = shell->paths[i][j];
			j++;
		}
		temp[j] = '/';
		temp[j + 1] = '\0';
		free(shell->paths[i]);
		shell->paths[i] = temp;
		j = 0;
		i++;
	}
}

// take a copy of paths to detect commands
void	get_paths(t_shell *shell)
{
	t_env	*current;

	if (shell->env == NULL)
		return ;
	current = shell->env;
	while (current)
	{
		if (ft_strncmp(current->variable, "PATH", 4) == 0 && current->content)
		{
			shell->paths = ft_split(current->content, ':');
			if (!shell->paths)
				exit(EXIT_FAILURE);
			add_backslash(shell);
			break ;
		}
		current = current->next;
	}
}

void	ft_parser(t_token *head, t_parser *parser, t_shell *shell)
{
	get_paths(shell);
	while (head)
	{
		parser->command_flag = 0;
		while (head->content[parser->index])
		{
			parser->filename_flag = 0;
			parser->eof_flag = 0;
			if (shell->paths && !parser->command_flag)
				detect_command(parser, head, shell->paths);
			detect_arguments(parser, head);
			detect_redirect(parser, head);
			detect_heredoc(parser, head);
			parser->index++;
			if (parser->filename_flag || parser->eof_flag)
			{
				if (detect_filename(parser, head))
					parser->index++;
			}
		}
		parser->index = 0;
		head = head->next;
	}
}
