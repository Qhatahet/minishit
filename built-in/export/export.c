/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:30:31 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/17 02:02:33 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	export_norm(t_token *token, t_export *export, t_shell *shell, int i)
{
	export->flag = 0;
	while (token->content[i][export->index]
		&& token->content[i][export->index] != '='
		&& token->content[i][export->index] != '+')
		export->index++;
	export->variable = ft_substr(token->content[i], 0, export->index);
	scan_env(shell, export);
	if (token->content[i][export->index] == '+')
	{
		export->flag++;
		export->index++;
	}
	export->content = ft_substr(token->content[i], export->index + 1,
			ft_strlen(token->content[i]) - export->index - 1);
	if (export->flag == 2)
		concat_content(shell, export);
	else if (export->flag == 1)
		edit_content(shell, export);
	else if (export->flag == -1)
		add_variable(shell, export);
	env_shlvl(shell);
}

void	export_command(t_shell *shell, t_token *token)
{
	t_export	*export;
	int			i;

	export = malloc(sizeof(t_export));
	if (!export)
		return ;
	if (token->content[1])
	{
		i = 1;
		while (token->content[i])
		{
			export->index = 0;
			if (ft_isdigit(token->content[i][export->index]))
			{
				i++;
				if (!token->content[i])
					break ;
			}
			export_norm(token, export, shell, i);
			i++;
		}
	}
	else
		sorted_print(shell->env);
	free(export);
}
