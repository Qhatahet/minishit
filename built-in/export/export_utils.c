/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:33:53 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/17 02:18:07 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_2d(char **a)
{
	int	i;

	if (a == NULL)
	{
		return ;
	}
	i = 0;
	while (a[i] != NULL)
	{
		free(a[i]);
		i++;
	}
	free(a);
}

int	env_count(t_env *env)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static char	**copy_env_to_array(t_env *env)
{
	int		i;
	char	**env_array;
	char	*tmp;
	t_env	*current;

	i = 0;
	current = env;
	env_array = malloc((env_count(env) + 1) * sizeof(char *));
	if (!env_array)
		return (NULL);
	while (current)
	{
		if (current->variable && current->content)
		{
			tmp = ft_strjoin(current->variable, "=\"");
			env_array[i] = ft_strjoin(tmp, current->content);
			free(tmp);
		}
		else
			env_array[i] = ft_strjoin(current->variable, "=\"");
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

char	**sort_env(char **env_array)
{
	int		j;
	int		i;
	char	*temp;

	i = 0;
	while (env_array[i])
	{
		j = i + 1;
		while (env_array[j])
		{
			if (ft_strcmp(env_array[i], env_array[j]) > 0)
			{
				temp = env_array[i];
				env_array[i] = env_array[j];
				env_array[j] = temp;
			}
			j++;
		}
		i++;
	}
	return (env_array);
}

void	sorted_print(t_env *env)
{
	char	**env_array;
	int		i;

	env_array = copy_env_to_array(env);
	if (!env_array)
	{
		return ;
	}
	env_array = sort_env(env_array);
	i = 0;
	while (env_array[i])
	{
		printf("declare -x %s\"\n", env_array[i]);
		i++;
	}
	ft_free_2d(env_array);
}
