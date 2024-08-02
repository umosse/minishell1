/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_ft_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:57 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/16 04:12:57 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void		ft_exec(t_data *data, t_cmd *cmd);
void		h_734_redirection(t_data *data, t_cmd *cmd);
void		ft_heredoc(t_data *data, t_cmd *cmd);
static char	*find_cmd(char *command, char **env);
static char	*find_parsing(char *command, char **env);

///////////////////////////////////////////////////////////////////////////////]
/*******************************************************************************
	takes a cmd link, redirect to correct fd, execute it, end the child
******************************************************************************/
void	ft_exec(t_data *data, t_cmd *cmd)
{
	char	*cmd_exe;

	if (!cmd)
		end(data, 0);
	sublim_child(data, cmd);
	h_734_redirection(data, cmd);
	child_builtin(data, cmd);
	cmd_exe = find_cmd(cmd->cmd_arg[0], data->env);
	if (!cmd_exe)
	{
		print_fd(2, ERR6"%s: not found\n", cmd->cmd_arg[0]);
		end(data, 127);
	}
	if (execve(cmd_exe, cmd->cmd_arg, data->env) == -1)
	{
		perror(ERR7"error execve");
		free_s(cmd_exe);
		end(data, 1);
	}
}

///////////////////////////////////////////////////////////////////////////////]
void	h_734_redirection(t_data *data, t_cmd *cmd)
{
	if (cmd->in_file && cmd->out_file && same_str(cmd->in_file, cmd->out_file))
		return (print_fd(2, ERR"same input\\output file '%s'\n", cmd->in_file), end(data, 3));
	if (cmd->in_file && cmd->in_bit == 1)
	{
		cmd->fd_in = open(cmd->in_file, O_RDONLY);
		if (cmd->fd_in < 0)
			return (perror(cmd->in_file), end(data, 5));
		dup_close(cmd->fd_in, STDIN_FILENO);
	}
	else if (cmd->in_file && cmd->in_bit == 2)
		ft_heredoc(data, cmd);
	if (cmd->out_file && cmd->out_bit)
	{
		if (cmd->out_bit == 1)
			cmd->fd_out = open(cmd->out_file, (O_WRONLY | O_CREAT | O_TRUNC), 0777);
		else if (cmd->out_bit == 2)
			cmd->fd_out = open(cmd->out_file, (O_WRONLY | O_CREAT | O_APPEND), 0777);
		if (cmd->fd_out < 0)
			return (perror(cmd->out_file), end(data, 5));
		dup_close(cmd->fd_out, STDOUT_FILENO);
	}
}


///////////////////////////////////////////////////////////////////////////////]
// create child that will pipe the input
void	ft_heredoc(t_data *data, t_cmd *cmd)
{
	int		fd[2];
	pid_t	pid;
	char	*tmp;

	if (pipe(fd) == -1)
		return (perror(ERR7"pipe"), end(data, 4));
	pid = fork();
	if (pid == -1)
		return (perror(ERR7"fork"), close(fd[0]), close(fd[1]), end(data, 4));
	if (!pid)
	{
		close(fd[0]);
		dup_close(data->fd_in_original, STDIN);
		data->fd_in_original = -1;
		while (1)
		{
			tmp = readline(C_415"heredoc:"RESET);
			if (!tmp || same_str(tmp, cmd->in_file))
				(close(fd[1]), free_s(tmp), end(data, 0));
			print_fd(fd[1], "%s\n", tmp);
			tmp = free_s(tmp);
		}
	}
	else
	{
		close(fd[1]);
		dup_close(fd[0], STDIN);
		waitpid(pid, NULL, 0);
	}
		// return (close(fd[1]), dup_close(fd[0], STDIN), waitpid(pid, NULL, 0), 0);
		// return (close(fd[1]), dup_close(fd[0], STDIN), wait(NULL), 0);
}


///////////////////////////////////////////////////////////////////////////////]
static char	*find_cmd(char *command, char **env)
{
	char	*rtrn;

	rtrn = NULL;
	if (!command)
	{
		return (NULL);
	}
	if (find_str(command, "/") >= 0)
	{
		if (access(command, X_OK) == 0)
			return (str("%1s", command));
		return (NULL);
	}
	rtrn = find_parsing(command, env);
	return (rtrn);
}

static char	*find_parsing(char *command, char **env)
{
	char	**paths;
	char	*cmd;
	int		i;

	paths = split(&rtrn_var(env, "PATH=")[5], ":");
	if (!paths)
		return (put(RED"ERROR--->$PATH:\n%t\n"RESET, env), NULL);
	cmd = NULL;
	i = -1;
	while (paths[++i])
	{
		cmd = str("%1s/%1s", paths[i], command);
		if (!access(cmd, X_OK))
			break ;
		cmd = free_s(cmd);
	}
	free_tab(paths);
	return (cmd);
}
