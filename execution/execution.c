/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:22:32 by qhatahet          #+#    #+#             */
/*   Updated: 2025/04/15 17:33:23 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	how_many_pipes(t_token *tokens)
{
	t_token *temp;
	int	i;

	i = 0;
	temp = tokens;
	while(temp)
	{
		temp = temp->next;
		i++;
	}
	return (i - 1);
}

int	is_there_command(t_token *tokens)
{
	t_token	*temp;
	int			i;

	temp = tokens;
	i = 0;
	while (temp->content[i])
	{
		if (*temp->type == COMMAND)
			return (1);
		i++;
	}
	return (0);
}

int	count_content(t_token *tokens)
{
	int	i;
	t_token	*temp;

	temp = tokens;
	i = 0;
	while(temp->content[i])
		i++;
	return (i);
}

int	count_env(t_env *env)
{
	int			i;
	t_env	*temp;

	i = 0;
	temp = env;
	while(temp != NULL)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

char	**get_env(t_env *env)
{
	t_env	*temp;
	char	**envp;
	int			i;

	temp = env;
	envp = ft_calloc((count_env(env) + 1), sizeof(char *));
	i = 0;
	while (temp)
	{
		temp->variable = ft_strjoin(temp->variable, "=");
		envp[i] = ft_strjoin(temp->variable, temp->content);
		temp = temp->next;
		i++;
	}
	return (envp);
}

void	execute_command(t_token *tokens, t_shell *shell)
{
	char	*cmd;
	char	**lst;
	t_token	*temp;
	int			i;

	temp = tokens;
	i = 0;
	lst = malloc((count_content(tokens) + 1) * sizeof(char *));
	if (!lst)
		return ;
	while (temp->content[i])
	{
		lst[i] = ft_strdup(temp->content[i]);
		i++;
	}
	lst[i] = NULL;
	get_paths(shell);
	int	id = fork();
	if (id == 0)
	{
		int	j = 0;
		if (shell->enviroment)
			ft_free_2d(shell->enviroment);
		if (!shell->enviroment)
			shell->enviroment = get_env(shell->env);
		while (shell->paths[j])
		{
			cmd = ft_strjoin(shell->paths[j], lst[0]);
			if (!cmd)
				exit(EXIT_FAILURE);
			if (!access(cmd, X_OK))
				break;
			j++;
		}
		ft_free_2d(shell->paths);
		if (shell->enviroment)
		{
			if((execve(cmd, lst, shell->enviroment)) == -1)
			{
				ft_free_2d(lst);
				ft_free_2d(shell->enviroment);
				printf("hello\n");
			}
		}
	}
	wait(NULL);
	if (lst)
		ft_free_2d(lst);
}

void	execute(t_shell *shell, t_token *tokens)
{
	(void)shell;
	if (!tokens->content)
		return ;
	if (how_many_pipes(tokens) > 0)
	{
		printf("there is pipes : %i\n", how_many_pipes(tokens));
	}
	else
	{
		// if(is_there_command(tokens))
			execute_command(tokens, shell);
	}
}
