/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_norm.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 01:57:29 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/17 02:05:15 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	scan_env(t_shell *shell, t_export *export)
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

void	concat_content(t_shell *shell, t_export *export)
{
	t_env	*current;
	char	*temp;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->variable, export->variable) == 0)
		{
			temp = ft_strjoin(current->content, export->content);
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

void	env_shlvl(t_shell *shell)
{
	t_env	*current;
	int		a;
	char	*temp;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->variable, "SHLVL") == 0)
		{
			a = ft_atoi(current->content);
			if (a > 999)
			{
				printf("ARRSH: shell level (%d) too high,resetting to 1\n",
					a);
				a = 1;
			}
			temp = ft_itoa(a);
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
