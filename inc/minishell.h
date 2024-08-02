/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2024/07/30 16:20:01 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <pthread.h>
# include <sys/stat.h>
# include <signal.h>
# include <dirent.h>
# include <termios.h>
// # include <termcap.h>
# include <sys/ioctl.h>
#include <readline/readline.h>
#include <readline/history.h>


# include "libft.h"





///////////////////////////////////////////////////////////////////////////////]
//
//			▒█▀▄▀█ ░█▀▀█ ▒█▄░▒█ ▒█▀▀▄ ░█▀▀█ ▀▀█▀▀ ▒█▀▀▀█ ▒█▀▀█ ▒█░░▒█
//			▒█▒█▒█ ▒█▄▄█ ▒█▒█▒█ ▒█░▒█ ▒█▄▄█ ░▒█░░ ▒█░░▒█ ▒█▄▄▀ ▒█▄▄▄█
//			▒█░░▒█ ▒█░▒█ ▒█░░▀█ ▒█▄▄▀ ▒█░▒█ ░▒█░░ ▒█▄▄▄█ ▒█░▒█ ░░▒█░░
//
///////////////////////////////////////////////////////////////////////////////]

# define STDIN 0
# define STDOUT 1
# define STDERR 1

typedef struct s_data	t_data;
typedef struct s_pip	t_pip;
typedef struct s_cmd	t_cmd;
typedef struct s_cmd2	t_pipeline;
typedef struct s_built_pair	t_built_pair;
typedef int (*t_builtin)(t_data *data, t_cmd *cmd);

typedef struct s_built_pair
{
	char		*cmd_name;
	t_builtin	cmd_exe;
}	t_built_pair;

typedef struct s_data
{
	char	**env;
	char	**history;
	int		fd_in_original;
	t_built_pair builtin_g[5];
	t_built_pair builtin[5];
	t_pipeline	*cmd;
	int		exit_code;
}	t_data;

typedef struct s_cmd
{
// 0 = nothing, 1 = <>, 2 = <<>>
	char	**cmd_arg;

	char	*in_file;// echo a < fichier; < 'fichier$5'
	// "fichier"
	char	*out_file;

	int		fd_in;// ne pas remplir
	int		fd_out;// ne pas remplir
	int		in_bit;
	int		out_bit;

	char	error;
	char	sublim;
	struct s_cmd	*next;
}	t_cmd;


typedef struct s_cmd2
{
	t_cmd	*cmd;
	struct s_cmd2	*next;

	char	and_or;//return & or |
	// int		error;
}	t_pipeline;

///////////////////////////////////////////////////////////////////////////////]
/********************************
		A
********************************/
int		ft_minishell(t_data *data);
/********************************
		P - PARSING
********************************/
int			ft_parsing(t_data *data, char *input);
char		*rtrn_var(char **env, char *to_extract);
char		*rtrn_var_v2(char **env, char *to_extract);
// 
char		*ft_extract_word(char *raw_line, int *i);
char		*ft_extract_words(char *raw_line, int *i);
char		*ft_extract_quotes(char *raw_line, int *i, char quote);
// 
void		sublim_child(t_data *data, t_cmd *cmd);
char		*ft_sublim(t_data *data, char *string);
t_cmd		*new_node(t_cmd *previous);
t_pipeline	*new_cmd(t_pipeline *previous, char c);
/********************************
		E - EXEC
********************************/
void	ft_exec(t_data *data, t_cmd *cmd);
void	h_734_redirection(t_data *data, t_cmd *cmd);
void	ft_heredoc(t_data *data, t_cmd *cmd);
// 
int		ft_minishell(t_data *data);
int		ft_exec_bigbro(t_data *data);
int		ft_big_brother(t_data *data, t_pipeline *cmd);
int		ft_child(t_data *data, t_cmd *cmd);
/********************************
		B - BUILTIN
********************************/
int		brother_builtin(t_data *data, t_cmd *cmd);
int		cmd_cd(t_data *data, t_cmd *cmd);
int		cmd_export(t_data *data, t_cmd *cmd);
int		cmd_unset(t_data *data, t_cmd *cmd);
int		cmd_alias(t_data *data, t_cmd *cmd);
// 
void	child_builtin(t_data *data, t_cmd *cmd);
int		cmd_echo(t_data *data, t_cmd *cmd);
int		cmd_type(t_data *data, t_cmd *cmd);
int		cmd_help(t_data *data, t_cmd *cmd);
int		cmd_miaou(t_data *data, t_cmd *cmd);
// 
void	replace_var(t_data *data, char *to_replace, char *replacement);
char	**pop_entry(char **env, char *to_pop);
/********************************
		T	TOOLS
********************************/
void		dup_close(int fd_replace, int fd_erase);
//
t_cmd		*new_node(t_cmd *previous);
t_pipeline	*new_cmd(t_pipeline *previous, char c);

/********************************
		Y
********************************/
int		initialization(int ac, char **av, char **env, t_data *data);
/********************************
		Z
********************************/
void	clear_cmd(t_data *data);
void	end(t_data *data, int exit_code);

#define INPUT_TXT "\033[0;31mm\033[0;32mi\033[0;33ma\033[0;34mo\033[0;35mu\e[0m > "
#define MSG_REDI "syntax error near unexpected token '%c'\n"


#endif
