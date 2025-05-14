/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 06:02:02 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/05 12:36:33 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_dollar(t_shell *shell, t_token *token, t_expand *expand, char *temp, int *i)
{
	t_env	*current;
	char	*digit;
	int		index;
	int		count;

	count = 0;
	expand->inner++;
	if(token->content[expand->outer][expand->inner] == ' ' || token->content[expand->outer][expand->inner] == '\0')
		temp[(*i)++] = '$';
	if(ft_isdigit(token->content[expand->outer][expand->inner]) && token->content[expand->outer][expand->inner] == '0')
	{
		digit = ft_strdup("ARSSH");
		expand->inner++;
		while(digit[count])
			temp[(*i)++] = digit[count++];
		free(digit);
	}
	else if(ft_isdigit(token->content[expand->outer][expand->inner]))
		expand->inner++;
	else
	{
		while(token->content[expand->outer][expand->inner] && (ft_isalnum(token->content[expand->outer][expand->inner]) || token->content[expand->outer][expand->inner] == '_'))
		{
			expand->inner++;
			count++;
		}
		expand->variable = ft_substr(token->content[expand->outer],expand->inner - count,count);
		printf("variable -> %s\n",expand->variable);
		current = shell->env;
		while (current)
		{
			if (ft_strcmp(current->variable, expand->variable) == 0)
			{
				index = 0;
				while(current->content[index])
					temp[(*i)++] = current->content[index++];
				return ;
			}
			current = current->next;
		}
		free(expand->variable);
		expand->variable = NULL;
	}
	if (expand->variable)
	{
		free(expand->variable);
		expand->variable = NULL;
	}
}

int		content_len(t_shell *shell,t_token *token, t_expand *expand)
{
	t_env	*current;

	int count = 0;
	expand->inner++;
	if(token->content[expand->outer][expand->inner] == ' ' || token->content[expand->outer][expand->inner] == '\0')
		count++;
	else if(token->content[expand->outer][expand->inner] == '0')
	{
		expand->inner++;
		count += 5;
	}
	else if(ft_isdigit(token->content[expand->outer][expand->inner]))
		expand->inner++;
	else
	{
		while(token->content[expand->outer][expand->inner] && (ft_isalpha(token->content[expand->outer][expand->inner]) || token->content[expand->outer][expand->inner] == '_'))
		{
			expand->inner++;
			count++;
		}
		expand->variable = ft_substr(token->content[expand->outer],expand->inner - count,count);
		current = shell->env;
		while (current)
		{
			if (ft_strcmp(current->variable, expand->variable) == 0)
			{
				count = ft_strlen(current->content);
				printf("count = %d\n",count);
				return (count);
			}
			current = current->next;
		}
	}
	return (count - ft_strlen(expand->variable));
}

int		count_length(t_shell *shell,t_token *token, t_expand *expand)
{
	expand->inner = 0;
	int size = 0;
	if (!token->content || !token->content[expand->outer])
    	return (0);
	while(expand->inner < (int)ft_strlen(token->content[expand->outer]) && token->content[expand->outer][expand->inner])
	{
		if(token->content[expand->outer][expand->inner] == '\"')
		{
			size++;
			expand->inner++;
			while(token->content[expand->outer][expand->inner] && token->content[expand->outer][expand->inner] != '\"')
			{
				if(token->content[expand->outer][expand->inner] == '$')
				{
					size += content_len(shell,token,expand) - 1;
					free(expand->variable);
					expand->variable = NULL;
				}
				expand->inner++;
				size++;
			}
		}
		if(token->content[expand->outer][expand->inner] == '\'')
		{
			size++;
			expand->inner++;
			while(token->content[expand->outer][expand->inner] && token->content[expand->outer][expand->inner] != '\'')
			{
				expand->inner++;
				size++;
			}
			size++;
			expand->inner++;
		}
		else if(token->content[expand->outer][expand->inner] == '$')
		{
			size += content_len(shell,token,expand);
			free(expand->variable);
			expand->variable = NULL;
		}
		else
		{
			expand->inner++;
			size++;
		}
	}
	expand->inner = 0;
	printf("expected size = %d\n",size);
	return (size);
}

void	expand_dollar(t_shell *shell,t_token *token, t_expand *expand)
{
	char	*temp;
	int		i;
	int		x;

	x = count_length(shell,token,expand);
	if((int)ft_strlen(token->content[expand->outer]) == x)
		return ;
	temp = malloc(x + 2);
	if(!temp)
		exit(130);
	i = 0;
	while(token->content[expand->outer][expand->inner])
	{
		if(token->content[expand->outer][expand->inner] == '\"')
		{
			temp[i++] = token->content[expand->outer][expand->inner++];
			while(token->content[expand->outer][expand->inner] && token->content[expand->outer][expand->inner] != '\"')
			{
				if(token->content[expand->outer][expand->inner] == '$')
				{
					handle_dollar(shell,token,expand,temp,&i);
					if (expand->variable)
					{
						free(expand->variable);
						expand->variable = NULL;
					}
				}	
				else
					temp[i++] = token->content[expand->outer][expand->inner++];
			}
		}
		if(token->content[expand->outer][expand->inner] == '\'')
		{
			temp[i++] = token->content[expand->outer][expand->inner++];
			while(token->content[expand->outer][expand->inner] && token->content[expand->outer][expand->inner] != '\'')
				temp[i++] = token->content[expand->outer][expand->inner++];
			temp[i++] = token->content[expand->outer][expand->inner++];
		}
		else if(token->content[expand->outer][expand->inner] == '$')
		{
			handle_dollar(shell,token,expand,temp,&i);
			if (expand->variable)
			{
				free(expand->variable);
				expand->variable = NULL;
			}
		}
		else
			temp[i++] = token->content[expand->outer][expand->inner++];
	}
	temp[i] = '\0';
	free(token->content[expand->outer]);
	token->content[expand->outer] = temp;
}
