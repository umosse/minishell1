/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_builtin_func.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/16 04:12:38 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	child_builtin(t_data *data, t_cmd *cmd);
int		cmd_echo(t_data *data, t_cmd *cmd);
int		cmd_type(t_data *data, t_cmd *cmd);
int		cmd_help(t_data *data, t_cmd *cmd);
int		cmd_miaou(t_data *data, t_cmd *cmd);

// static int skip_duplicate_char()
// {
// 	return (0);
// }
///////////////////////////////////////////////////////////////////////////////]
// 	cmd_echo; cmd_type; cmd_help; cmd_miaou;
void	child_builtin(t_data *data, t_cmd *cmd)
{
	t_built_pair	*ptr;
	
	if (brother_builtin(data, cmd))
		end(data, WEXITSTATUS(data->exit_code));
	else if (same_str(cmd->cmd_arg[0], "env"))
		(put("%-t", data->env), end(data, 0));
	else if (same_str(cmd->cmd_arg[0], "history"))
		(put("%-t", data->history), end(data, 0));
	else if (same_str(cmd->cmd_arg[0], "pwd"))
		(put("%s\n", rtrn_var(data->env, "PWD=") + 4), end(data, 0));
	ptr = data->builtin - 1;
	while ((++ptr)->cmd_exe && ptr->cmd_name)
	{
		if (same_str(cmd->cmd_arg[0], ptr->cmd_name))
		{
			data->exit_code = ptr->cmd_exe(data, cmd);
			end(data, WEXITSTATUS(data->exit_code));
		}
	}
}


int	cmd_echo(t_data *data, t_cmd *cmd)
{
	int	i;
	int	b;
	(void)data;
	b = 1;
	if (!find_str(cmd->cmd_arg[1], "-n") && cmd->cmd_arg[2])
	{
		i = 2;
		b++;
		while (cmd->cmd_arg[i] && cmd->cmd_arg[i + 1])
			i++;
		cmd->cmd_arg[i] = join(cmd->cmd_arg[i], "$\n", 0b10, 0);
	}
	if (cmd->cmd_arg[b])
		put("%#.1t\n", &cmd->cmd_arg[b]);
	end(data, 0);
	return (0);
}

int	cmd_type(t_data *data, t_cmd *cmd)
{
	(void)data;
	(void)cmd;
	return (0);
}

int	cmd_help(t_data *data, t_cmd *cmd)
{
	(void)data;
	(void)cmd;
	return (0);
}

int	cmd_miaou(t_data *data, t_cmd *cmd)
{
	ft_print_cat(data->exit_code, "hiiiiii", 0b10);
	(void)cmd;
	return (0);
}