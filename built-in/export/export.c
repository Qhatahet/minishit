/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:30:31 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/28 22:04:36 by qais             ###   ########.fr       */
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

void	concat_content(t_shell *shell,t_export *export)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->variable, export->variable) == 0)
		{
			char *temp = ft_strjoin(current->content,export->content);
			free(current->content);
			current->content = temp;
			return ;
		}
		current = current->next;
	}
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

void env_shlvl(t_shell *shell)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->variable, "SHLVL") == 0)
		{
			int a = ft_atoi(current->content);
			if(a > 999)
			{
				printf("arrsh: warning: shell level (%d) too high, resetting to 1\n",a);
				a = 1;
			}
			char *temp = ft_itoa(a);
			free(current->content);
			current->content = temp;
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
	t_export *export;
	
	export = malloc(sizeof(t_export));
	if(!export)
		return ;
	if(token->content[1])
	{
		int i = 1;
		while(token->content[i])
		{
			export->index = 0;
			export->flag = 0;
			if(ft_isdigit(token->content[i][export->index]))
			{
				i++;
				if(!token->content[i])
					break;
			}
			while(token->content[i][export->index] && token->content[i][export->index] != '=' && token->content[i][export->index] != '+')
				export->index++;
			export->variable = ft_substr(token->content[i],0,export->index);
			scan_env(shell,export);
			if(token->content[i][export->index] == '+')
			{
				export->flag++;
				export->index++;
			}
			export->content = ft_substr(token->content[i],export->index + 1,ft_strlen(token->content[i]) - export->index - 1);
			if(export->flag == 2)
				concat_content(shell,export);
			else if(export->flag == 1)
				edit_content(shell, export);
			else if(export->flag == -1)
				add_variable(shell,export);
			env_shlvl(shell);
			i++;
		}
	}
	else
		sorted_print(shell->env);
	free(export);

}
