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

int	brother_builtin(t_data *data, t_cmd *cmd);
int	cmd_cd(t_data *data, t_cmd *cmd);
int	cmd_export(t_data *data, t_cmd *cmd);
int	cmd_unset(t_data *data, t_cmd *cmd);
int	cmd_alias(t_data *data, t_cmd *cmd);

///////////////////////////////////////////////////////////////////////////////]
// cmd_cd; cmd_export; cmd_unset; cmd_alias;
int	brother_builtin(t_data *data, t_cmd *cmd)
{
	t_built_pair	*ptr;

	if (!cmd)
		return (0);
	sublim_child(data, cmd);
	if (same_str(cmd->cmd_arg[0], "exit"))
		end(data, 0);
	ptr = data->builtin_g - 1;
	while ((++ptr)->cmd_exe && ptr->cmd_name)
	{
		if (same_str(cmd->cmd_arg[0], ptr->cmd_name))
		{
			data->exit_code = ptr->cmd_exe(data, cmd);
			return (1);
		}
	}
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
#define BUF_WD 1024
// HOME=/home/kalipso
// SHELL=/usr/bin/zsh
int	cmd_cd(t_data *data, t_cmd *cmd)
{
	char	*new_pwd;
	char	*new_oldpwd;
	char	buf_newpath[BUF_WD];

	if (!cmd->cmd_arg[1])
		cmd->cmd_arg = expand_tab(cmd->cmd_arg, str("%1s", rtrn_var(data->env, "HOME=") + 5));

	if (chdir(cmd->cmd_arg[1]))
		return (perror(ERR9"cd bad dirrrecctory?:"), 1);
	ft_memset(buf_newpath, 0, BUF_WD);
	getcwd(buf_newpath, BUF_WD);
	if (!*buf_newpath)
		return (perror(ERR9"getcwd"), 1);
	new_pwd = str("PWD=%1s", buf_newpath);
	new_oldpwd = str("OLDPWD=%1s", &(rtrn_var(data->env, "PWD=")[4]));
	replace_var(data, "PWD=", new_pwd);
	replace_var(data, "OLDPWD=", new_oldpwd);
	return (0);
}
// export var="text"
int	cmd_export(t_data *data, t_cmd *cmd)
{
	char	*first_arg = cmd->cmd_arg[1];
	// put(C_123"first_arg=%s\n", first_arg);
	if (!cmd->cmd_arg[1] || wii('=', first_arg) <= 0)
		return (put(ERR0"bad assignement\n"), 1);

	char	*var = str("%1.*s", len_m(first_arg, "=") + 1, first_arg);
	// put(C_403"var=%s\n", var);
	// char	*value = str("%1s", &first_arg[l]);
	replace_var(data, var, str("%1s", first_arg));
	free_s(var);
	// if (!cmd->cmd_arg[1])
	// 	return (put("%t", data->env), (void)0);
	// 	// cmd->cmd_arg = expand_tab(cmd->cmd_arg, str("%1s", rtrn_var(data->env, "HOME=") + 5));



	// new_pwd = str("PWD=%1s", buf_newpath);
	// new_oldpwd = str("OLDPWD=%1s", &(rtrn_var(data->env, "PWD=")[4]));

	// replace_var(data, "PWD=", new_pwd);
	// replace_var(data, "OLDPWD=", new_oldpwd);
	return (0);
	// end(data, 0);
}

int	cmd_unset(t_data *data, t_cmd *cmd)
{
	char	*first_arg = cmd->cmd_arg[1];
	if (!first_arg)
		return (put(ERR0"unset: not enough arguments\n"), -1);

	char	*var = str("%1s=", first_arg);
	// char	*value = str("%1s", &first_arg[l]);
	data->env = pop_entry(data->env, var);
	free_s(var);
	return (0);
}

int	cmd_alias(t_data *data, t_cmd *cmd)
{
	(void)data;
	(void)cmd;
	return (0);
}
