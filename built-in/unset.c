/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 21:54:42 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/01 21:54:50 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_node(t_shell *shell, t_export *export)
{
    t_env	*current;
    t_env	*prev;

    current = shell->env;
    prev = NULL;
    while (current)
    {
        if (ft_strcmp(current->variable, export->variable) == 0)
        {
            if (prev == NULL)
                shell->env = current->next;
            else
                prev->next = current->next;
            free(current->content);
            free(current->variable);
            free(current);
            return ;
        }
        prev = current;
        current = current->next;
    }
}

void	unset_command(t_shell *shell, t_token *token)
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
				delete_node(shell, export);
			else if(export->flag == -1)
				return ;
		}
	}
}