/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:22:32 by qhatahet          #+#    #+#             */
/*   Updated: 2025/04/18 16:26:00 by qhatahet         ###   ########.fr       */
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
	char	*str;
	int			i;

	temp = env;
	envp = ft_calloc((count_env(env) + 1), sizeof(char *));
	i = 0;
	while (temp)
	{
		char *t3res_test;
		str = malloc(ft_strlen(temp->variable) + ft_strlen(temp->content) + 2);
		t3res_test = str;
		str = ft_strjoin(temp->variable, "=");
		free(t3res_test);
		t3res_test = str;
		str = ft_strjoin(str, temp->content);
		free (t3res_test);
		envp[i] = ft_strdup(str);
		free(str);
		temp = temp->next;
		i++;
	}
	envp[i] = NULL;
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
	int	id = fork();
	if (id == 0)
	{
		int	j = 0;
		get_paths(shell);
		if (shell->enviroment)
			ft_free_2d(shell->enviroment);
		if (!shell->enviroment)
			shell->enviroment = get_env(shell->env);
		while (shell->paths[j] && shell->paths)
		{
			cmd = ft_strjoin(shell->paths[j], lst[0]);
			// printf("%s\n", cmd);
			if (!cmd)
				exit(EXIT_FAILURE);
			if (!access(cmd, X_OK))
				break;
			free(cmd);
			cmd = NULL;
			j++;
		}
		// printf("%s\n", cmd);
		ft_free_2d(shell->paths);
		if (!cmd)
		{
			ft_printf("command not found\n");
			if (lst)
				ft_free_2d(lst);
			if(shell->enviroment)
			{
				ft_free_2d(shell->enviroment);
				shell->enviroment = NULL;
			}
			exit(EXIT_FAILURE);
		}
		if((execve(cmd, lst, shell->enviroment)) == -1)
		{
			// ft_free_2d(shell->enviroment);
			// ft_free_2d(lst);
			printf("hello\n");
		}
	}
	wait(NULL);
	i = 0;
	if (lst && lst[i])
	{
		while (lst[i])
		{
			free(lst[i]);
			i++;
		}
		free(lst);
	}
	if(shell->enviroment)
	{
		ft_free_2d(shell->enviroment);
		shell->enviroment = NULL;
	}
	// if (shell->paths[0] && shell->paths)
	// {
	// 	ft_free_2d(shell->paths);
	// 	shell->paths = NULL;
	// }
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
		// if (shell->paths)
		// {
		// 	ft_free_2d(shell->paths);
		// 	shell->paths = NULL;
		// }
	}
}
