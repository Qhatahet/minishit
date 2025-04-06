/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 05:18:33 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/01 18:20:11 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// print env
void	print_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (current->variable && current->content)
			printf("%s=%s\n", current->variable, current->content);
		else if (current->variable)
			printf("%s=\n", current->variable);
		current = current->next;
	}
}
