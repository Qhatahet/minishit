/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:05:04 by qhatahet          #+#    #+#             */
/*   Updated: 2025/05/20 23:29:54 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	qoutes_counter(char *string)
{
	int count;
    int index;
    char quote;

	count = 0;
    index = 0;
    quote = '\0';
	while(string[index])
	{
		if(string[index] == '\'' || string[index] == '\"')
		{
			quote = string[index];
			count++;
			index++;
			break ;
		}
		index++;
	}
	while (string[index])
	{
		if(count % 2 == 0)
		{
			while(string[index])
			{
				if(string[index] == '\'' || string[index] == '\"')
				{
					quote = string[index];
					count++;
					index++;
					break ;
				}
				index++;
			}
		}
		if (string[index] == quote)
			count++;
		index++;
	}
	return(count);
}

char *remove_qoutes(char *string, t_shell *shell)
{
	char	*temp;
	int		i;
	int count;
    int index;
    char quote; 

	// printf("quotes count = %d\n" ,count_quotes(string));
	temp = malloc(ft_strlen(string) - qoutes_counter(string) + 1);
	if(!temp)
		exit(123);
	index = 0;
	i = 0;
	count = 0;
	shell->expand_flag = 1;
	quote = '\0';
	while(string[index])
	{
		if(string[index] == '\'' || string[index] == '\"')
		{
			quote = string[index];
			count++;
			index++;
			break ;
		}
		else
			temp[i++] = string[index++];
	}
	while (string[index])
	{
		if(count % 2 == 0)
		{
			while(string[index])
			{
				if(string[index] == '\'' || string[index] == '\"')
				{
					quote = string[index];
					count++;
					index++;
					break ;
				}
				temp[i++] = string[index++];
			}
		}
		if (string[index] == quote)
		{
			count++;
			index++;
		}
		else
			temp[i++] = string[index++];
	}
	temp[i] = '\0';
	free(string);
	return (temp);
}

static void	dollar(char *text,t_shell *shell,char *temp,int *i)
{
	t_env *current;

	int count = 0;
	shell->temp_index++;
	if(text[shell->temp_index] == ' ' || text[shell->temp_index] == '\0')
		temp[(*i)++] = '$';
	if(ft_isdigit(text[shell->temp_index]) && text[shell->temp_index] == '0')
	{
		char *digit = ft_strdup("ARSSH");
		shell->temp_index++;
		while(digit[count])
			temp[(*i)++] = digit[count++];
		free(digit);
	}
	else if(ft_isdigit(text[shell->temp_index]))
		shell->temp_index++;
	else
	{
		while(text[shell->temp_index] && (ft_isalpha(text[shell->temp_index]) || text[shell->temp_index] == '_'))
		{
			shell->temp_index++;
			count++;
		}
		shell->variable = ft_substr(text,shell->temp_index - count,count);
		current = shell->env;
		while (current)
		{
			if (ft_strcmp(current->variable, shell->variable) == 0)
			{
				int index = 0;
				while(current->content[index])
					temp[(*i)++] = current->content[index++];
				free(shell->variable);
				shell->variable = NULL;
				return ;
			}
			current = current->next;
		}
		free(shell->variable);
		shell->variable = NULL;
	}
}

int		length(char *text ,t_shell *shell)
{
	t_env	*current;

	int count = 0;
	shell->temp_index++;
	if(text[shell->temp_index] == ' ' || text[shell->temp_index] == '\0')
		count++;
	else if(text[shell->temp_index] == '0')
	{
		shell->temp_index++;
		count += 5;
	}
	else if(ft_isdigit(text[shell->temp_index]))
		shell->temp_index++;
	else
	{
		while(text[shell->temp_index] && (ft_isalpha(text[shell->temp_index]) || text[shell->temp_index] == '_'))
		{
			shell->temp_index++;
			count++;
		}
		shell->variable = ft_substr(text,shell->temp_index - count,count);
		current = shell->env;
		while (current)
		{
			if (ft_strcmp(current->variable, shell->variable) == 0)
			{
				count = ft_strlen(current->content);
				return (count);
			}
			current = current->next;
		}
	}
	return (count - ft_strlen(shell->variable));
}

int		string_len(char *text ,t_shell *shell)
{
	shell->temp_index = 0;
	int size = 0;
	if (!text)
    	return (0);
	while(shell->temp_index < (int)ft_strlen(text) && text[shell->temp_index])
	{
		if(text[shell->temp_index] == '$')
		{
			size += length(text,shell);
			free(shell->variable);
			shell->variable = NULL;
		}
		else
		{
			shell->temp_index++;
			size++;
		}
	}
	shell->temp_index = 0;
	return (size);
}

char	*expand_heredoc(char *text, t_shell *shell) // expand $ in all cases ('', "", )
{
	char	*temp;
	int		i;
	int		x;

	x = string_len(text,shell);
	if((int)ft_strlen(text) == x)
		return (text);
	temp = malloc(x + 1);
	if(!temp)
		exit(130);
	i = 0;
	while(text[shell->temp_index])
	{
		if(text[shell->temp_index] == '$')
			dollar(text,shell,temp,&i);
		else
			temp[i++] = text[shell->temp_index++];
	}
	temp[i] = '\0';
	free(text);
	return (temp);
}


int	check_text(char *text, t_shell *shell)
{
	t_env	*temp;
	// char	*temp_text;
	// int			i;

	temp = shell->env;
	while (temp)
	{
		if (!ft_strcmp(text, temp->variable))
			return (1);
		temp = temp->next;
	}
	return (0);
}

static void heredoc_signal_handler(int sig)
{
    (void)sig;
    rl_redisplay();
    write(1, "^C\n", 3);
	rl_on_new_line();
    exit(128 + SIGINT);
}

void    open_heredoc(t_shell *shell, char **lst, t_fds *fds)
{
    int         i;
    int         j;
    char        *delimiter;
    char        *text;
    pid_t       pid;
    int         status;
    struct sigaction sa;

    delimiter = NULL;
    i = 0;
    fds->temp = ft_strdup(".temp");
    j = 0;
    fds->flag_expand = 1;
	fds->fd_heredoc = 0;
    struct sigaction original_sa;
    sigaction(SIGINT, NULL, &original_sa);
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    while (lst[i])
    {
        if (!ft_strcmp(lst[i], "<<"))
        {
            i++;
            pid = fork();
            if (pid == 0)
            {
                sa.sa_handler = heredoc_signal_handler;
                sa.sa_flags = 0;
                sigemptyset(&sa.sa_mask);
                sigaction(SIGINT, &sa, NULL);
                fds->fd_in[j] = open(fds->temp, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (fds->fd_in[j] < 0)
                    exit(1);
                delimiter = ft_strdup(lst[i]);
                if (!delimiter)
                    exit(1);
                if (delimiter[0] == '\'' || delimiter[0] == '\"')
                    delimiter = remove_qoutes(delimiter, shell);
                while (1)
                {
                    text = readline("> ");
                    if (!text)
                    {
                        char *tmp = ft_strjoin("ARSSH: warning: here-document delimited by end-of-file ( wanted `", delimiter);
                        char *t = ft_strjoin(tmp, "\')\n");
                        free(tmp);
                        write(1, t, ft_strlen(t));
                        free(t);
                        free(delimiter);
                        close(fds->fd_in[j]);
						unlink(".temp");
                        exit(0);
                    }
                    if (shell->expand_flag)
                        text = expand_heredoc(text, shell);
                    if (!ft_strcmp(text, delimiter))
                    {
                        free(text);
                        free(delimiter);
                        close(fds->fd_in[j]);
                        exit(0);
                    }
                    write(fds->fd_in[j], text, ft_strlen(text));
                    write(fds->fd_in[j], "\n", 1);
                    free(text);
                }
                free(delimiter);
            }
            else if (pid > 0)
            {
                waitpid(pid, &status, 0);
                sigaction(SIGINT, &original_sa, NULL);
                if (WIFEXITED(status))
                {
                    g_exit_status = WEXITSTATUS(status);
                    if (g_exit_status == 128 + SIGINT)
                        return ;
                }
                else if (WIFSIGNALED(status))
                {
                    g_exit_status = 128 + WTERMSIG(status);
                    return ;
                }
            }
            else
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            j++;
        }
        i++;
    }
    fds->flag_heredoc = 1;
}
