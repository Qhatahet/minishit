/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 02:54:32 by qais              #+#    #+#             */
/*   Updated: 2025/05/03 06:08:27 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_oldpwd(t_shell *shell, char *cwd)
{
	t_env	*oldpwd;

	oldpwd = shell->env;
	while (ft_strcmp(oldpwd->variable, "OLDPWD"))
		oldpwd = oldpwd->next;
	if (!ft_strcmp(oldpwd->variable, "OLDPWD"))
	{
		free (oldpwd->content);
		oldpwd->content = ft_strdup(cwd);
	}
}

void	change_pwd(t_shell *shell, char *s, char *oldpwd)
{
	t_env	*pwd;
	char	cwd[1024];

	pwd = shell->env;
	if (chdir(s) == -1)
	{
		printf("hello there : wrong directory\n");
		return ;
	}
	change_oldpwd(shell, oldpwd);
	while (ft_strcmp(pwd->variable, "PWD"))
		pwd = pwd->next;
	if (!ft_strcmp(pwd->variable, "PWD"))
	{
		free(pwd->content);
		pwd->content = NULL;
		pwd->content = ft_strdup(getcwd(cwd, 1024));
	}
}

char	*grep_home(t_shell *shell)
{
	t_env	*temp;

	temp = shell->env;
	while (temp)
	{
		if (!ft_strcmp(temp->variable, "HOME"))
			return (temp->content);
		temp = temp->next;
	}
	return (NULL);
}

void	ft_cd(t_shell *shell, t_token *token)
{
	t_token	*tmp;
	char	cwd[1024];
	int		i;

	i = 0;
	tmp = token;
	if ((getcwd(cwd, 1024)) == NULL)
		return ;
	if (!ft_strcmp(tmp->content[i], "cd"))
		i++;
	if (tmp->content[i] && ft_strcmp(tmp->content[i], "~"))
	{
		if (!access(tmp->content[i], X_OK))
			change_pwd(shell, tmp->content[i], cwd);
		else
			printf("arssh: cd: %s: No such file or directory\n", tmp->content[i]);
	}
	else if (!tmp->content[i] || !ft_strcmp(tmp->content[i], "~"))
		change_pwd(shell, grep_home(shell), cwd);
	else
		perror("cd");
}
