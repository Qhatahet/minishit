/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:31:16 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/28 21:19:26 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_variable(t_shell *shell, t_env *temp, char **env)
{
	int	end;

	end = 0;
	while (env[shell->temp_index][end] != '=' && env[shell->temp_index][end])
		end++;
	temp->variable = ft_substr(env[shell->temp_index], 0, end);
	if (!temp->variable)
		return ;
}

void	copy_content(t_shell *shell, t_env *temp, char **env)
{
	int	end;
	int	start;

	end = 0;
	start = 0;
	while (env[shell->temp_index][start] != '='
		&& env[shell->temp_index][start])
		start++;
	while (env[shell->temp_index][end])
		end++;
	start++;
	temp->content = ft_substr(env[shell->temp_index], start, end);
	if (!temp->content)
		return ;
}

t_env	*create_env_node(void)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		exit(EXIT_FAILURE);
	new_node->next = NULL;
	return (new_node);
}

// enviroment copy
void	env_copy(t_shell *shell, char **env)
{
	t_env	*temp;

	if (env[0] == NULL)
		return ;
	shell->env = create_env_node();
	if (!shell->env)
		return ;
	temp = shell->env;
	shell->temp_index = 0;
	while (env[shell->temp_index])
	{
		copy_variable(shell, temp, env);
		copy_content(shell, temp, env);
		if (env[shell->temp_index + 1])
		{
			temp->next = create_env_node();
			temp = temp->next;
		}
		shell->temp_index++;
	}
	env_edit(shell);
	shell->temp_index = 0;
}
