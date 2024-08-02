/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_exec_family.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:25 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/16 04:12:25 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_minishell(t_data *data);
int	ft_exec_bigbro(t_data *data);
int	ft_big_brother(t_data *data, t_pipeline *cmd);
int	ft_child(t_data *data, t_cmd *cmd);

#define INPUT_TEXT " \033[38;5;%dmm\033[38;5;%dmi\033[38;5;%dma\033[38;5;%dmo\033[38;5;%dmu\e[0m > "
///////////////////////////////////////////////////////////////////////////////]
// 1 loop per input
int	ft_minishell(t_data *data)
{
	char	*input;
	char	*input_txt;

	input_txt = str(INPUT_TEXT, rand() % 256, rand() % 256, rand() % 256, rand() % 256, rand() % 256);
	input = readline(input_txt);
	free_s(input_txt);
	input = trim(input, " \n\t");
	if (!input)
		return (0);
	if (ft_parsing(data, input))
		return (clear_cmd(data), 0);
	data->history = expand_tab(data->history, input);
	add_history(input);
	return (ft_exec_bigbro(data));
}

///////////////////////////////////////////////////////////////////////////////]
/*******************************************************************************
	takes data->cmd with arguments,
		create the big_brother that will do 1 pipeline
	return only:
		0 on succcess and -1 to exit (for now)
******************************************************************************/
int	ft_exec_bigbro(t_data *data)
{
	t_pipeline	*cmd;
	int		rtrn;

	cmd = data->cmd;
	if (!data->cmd)
		return (perror(ERR3"empty args struct"), 1);
	while (cmd)
	{
		if (!cmd->and_or || (cmd->and_or == '&' && !rtrn) || (cmd->and_or == '|' && rtrn))
			ft_big_brother(data, cmd);
		rtrn = WEXITSTATUS(data->exit_code);
		if (rtrn == 255)
			return (clear_cmd(data), rtrn);
		cmd = cmd->next;
	}
	return (clear_cmd(data), 0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		does 1 chained list
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int	ft_big_brother(t_data *data, t_pipeline *pipeline)
{
	pid_t	pid;
	t_cmd	*cmd;

	cmd = pipeline->cmd;
	if (brother_builtin(data, cmd))
		cmd = cmd->next;
	if (!cmd)
		return (0);
	pid = fork();
	if (pid == -1)
		return (perror(ERR7"fork"), 1);
	if (!pid)
	{
		while (cmd && cmd->next)
		{
			if (ft_child(data, cmd))
				end(data, WEXITSTATUS(data->exit_code));
			cmd = cmd->next;
		}
		ft_exec(data, cmd);
	}
	else
		waitpid(pid, &data->exit_code, 0);
	return (0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		takes a cmd node, return the return value of child
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int	ft_child(t_data *data, t_cmd *cmd)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		return (perror(ERR7"pipe"), 1);
	pid = fork();
	if (pid == -1)
		return (perror(ERR7"fork"), close(fd[0]), close(fd[1]), 2);
	if (!pid)
	{
		close(fd[0]);
		dup_close(fd[1], STDOUT);
		ft_exec(data, cmd);
	}
	else
	{
		close(fd[1]);
		dup_close(fd[0], STDIN);
		waitpid(pid, &data->exit_code, 0);
	}
	return (0);
}
