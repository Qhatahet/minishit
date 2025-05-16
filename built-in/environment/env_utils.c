/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 05:18:33 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/16 16:34:20 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_shell *shell ,t_env *env)
{
	t_env *current = env;
	size_t total_len = 0;
	
	while (current)
	{
		if (current->variable)
			total_len += ft_strlen(current->variable);
		if (current->content)
			total_len += ft_strlen(current->content);
		total_len += 2;
		current = current->next;
	}
	char *result = malloc(total_len + 1);
	if (!result)
		return ;
	result[0] = '\0';
	current = env;
	while (current)
	{
		if (current->variable)
			ft_strlcat(result, current->variable, total_len + 1);
		ft_strlcat(result, "=", total_len + 1);
		if (current->content)
			ft_strlcat(result, current->content, total_len + 1);
		ft_strlcat(result, "\n", total_len + 1);
		current = current->next;
	}
	ft_putstr_fd(result, shell->fd_out);
	free(result);
}

void	env_edit(t_shell *shell)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->variable, "SHLVL") == 0)
		{
			int a = ft_atoi(current->content);
			a++;
			if(a > 999)
			{
				printf("arssh: warning: shell level (%d)", a);
				printf("too high, resetting to 1\n");
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
