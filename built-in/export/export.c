/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:30:31 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/01 21:38:35 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	scan_env(t_shell *shell,t_export *export)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->variable, export->variable) == 0)
		{
			export->flag = 1;
			return ;
		}
		current = current->next;
	}
	export->flag = -1;
}

void	edit_content(t_shell *shell, t_export *export)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->variable, export->variable) == 0)
		{
			free(current->content);
			current->content = export->content;
			return ;
		}
		current = current->next;
	}
}

void	add_variable(t_shell *shell, t_export *export)
{
    t_env	*current;
    t_env	*new_node;

    current = shell->env;
    new_node = create_env_node();
    if (!new_node)
        return ;
    new_node->variable = export->variable;
    new_node->content = export->content;
    new_node->next = NULL;
    if (!current)
    {
        shell->env = new_node;
        return ;
    }
    while (current->next)
        current = current->next;
    current->next = new_node;
}


void	export_command(t_shell *shell,t_token *token)
{
	shell->temp_index = 1;
	while (token->content[shell->temp_index])
		shell->temp_index++;
	if(shell->temp_index > 2)
		return ;
	else
	{
		t_export *export;

		export = ft_calloc(1,sizeof(t_export));
		if(!export)
			return ;
		if(token->content[1])
		{
			while(token->content[1][export->index] && token->content[1][export->index] != '=')
				export->index++;
			export->variable = ft_substr(token->content[1],0,export->index);
			export->content = ft_substr(token->content[1],export->index + 1,ft_strlen(token->content[1]) - export->index - 1);
			scan_env(shell,export);
			if(export->flag == 1)
				edit_content(shell, export);
			else if(export->flag == -1)
				add_variable(shell,export);
		}
		else
			sorted_print(shell->env);
	}
}
