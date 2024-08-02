/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Y_init.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 06:21:51 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/30 13:39:50 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int			initialization(int ac, char **av, char **env, t_data *data);
static void	copy_env(t_data *data, char **env);
static void	ini_builtin(t_data *data);

///////////////////////////////////////////////////////////////////////////////]
// ini
int	initialization(int ac, char **av, char **env, t_data *data)
{
	(void)av;
	srand(time(NULL));
	if (ac != 1)
		return (put(ERR"Wrong number of arguments\n"), exit(0), 1);
	ft_memset(data, 0, sizeof(t_data));
	copy_env(data, env);
	ini_builtin(data);
	data->fd_in_original = dup(STDIN_FILENO);
	// nuber of shell, makefile shell, ...
	return (0);
}

static void	copy_env(t_data *data, char **env)
{
	int		i;
	char	*string;

	if (!env)
		return ;
	i = -1;
	while (env[++i])
	{
		string = str("%1s", env[i]);
		data->env = expand_tab(data->env, string);
	}
}

static void	ini_builtin(t_data *data)
{
	data->builtin_g[0].cmd_name = "cd";
	data->builtin_g[0].cmd_exe = cmd_cd;
	data->builtin_g[1].cmd_name = "export";
	data->builtin_g[1].cmd_exe = cmd_export;
	data->builtin_g[2].cmd_name = "unset";
	data->builtin_g[2].cmd_exe = cmd_unset;
	data->builtin_g[3].cmd_name = "alias";
	data->builtin_g[3].cmd_exe = cmd_alias;
	data->builtin[0].cmd_name = "echo";
	data->builtin[0].cmd_exe = cmd_echo;
	data->builtin[1].cmd_name = "type";
	data->builtin[1].cmd_exe = cmd_type;
	data->builtin[2].cmd_name = "help";
	data->builtin[2].cmd_exe = cmd_help;
	data->builtin[3].cmd_name = "miaou";
	data->builtin[3].cmd_exe = cmd_miaou;
}
