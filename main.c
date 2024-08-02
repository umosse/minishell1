/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 16:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/30 13:15:34 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

/*******************************************************************************
******************************************************************************/
// <!> - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  </!>
///////////////////////////////////////////////////////////////////////////////]
/*******************************************************************************

			░█████╗░  ███████╗░█████╗░██╗██████╗░███████╗
			██╔══██╗  ██╔════╝██╔══██╗██║██╔══██╗██╔════╝
			███████║  █████╗░░███████║██║██████╔╝█████╗░░
			██╔══██║  ██╔══╝░░██╔══██║██║██╔══██╗██╔══╝░░
			██║░░██║  ██║░░░░░██║░░██║██║██║░░██║███████╗
			╚═╝░░╚═╝  ╚═╝░░░░░╚═╝░░╚═╝╚═╝╚═╝░░╚═╝╚══════╝


cd: Change directory.
exit: Terminate the shell.
echo: Print arguments to the standard output.
pwd: Print current working directory.
export: Set environment variables.
unset: Unset environment variables.
history: Display command history.
alias: Define or display aliases.
type: Display information about commands (alias, built-in, executable).
help: Display help information about built-in commands or shell features.



	waitpid(pid, &status, 0)
WIFEXITED(status): Returns true if the child terminated normally (by calling exit or returning from main).
WEXITSTATUS(status): Returns the exit status of the child. This should only be used if WIFEXITED(status) is true.
WIFSIGNALED(status): Returns true if the child process was terminated by a signal.
WTERMSIG(status): Returns the signal number that caused the child to terminate. This should only be used if WIFSIGNALED(status) is true.


BAD TEST
	echo -nnd salut
	E'C'HO
	echo a | grep a

	< !!! >		ccmd that start or end with && ||
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////]
int	main(int ac, char **av, char **env)
{
	t_data	data;
	int		rtrn;

	initialization(ac, av, env, &data);
	while (1)
	{
		rtrn = ft_minishell(&data);
		if (WEXITSTATUS(rtrn) == 255)
			break ;
	}
	end(&data, 0);
	return (0);
}


/*******************************************************************************
unistd.h Functions

    int isatty(int fd);
        Purpose: Checks if a file descriptor refers to a terminal.
        Usage: Useful for determining if the input/output is from/to a terminal or another source, such as a file or pipe.
        Example: Check if stdin is a terminal.

        c

    if (isatty(STDIN_FILENO)) {
        printf("Standard input is a terminal.\n");
    } else {
        printf("Standard input is not a terminal.\n");
    }

char *ttyname(int fd);

    Purpose: Returns a string with the file name of the terminal connected to the file descriptor.
    Usage: Identify the terminal associated with a file descriptor.
    Example:

    c

    char *term_name = ttyname(STDIN_FILENO);
    if (term_name) {
        printf("Terminal name: %s\n", term_name);
    } else {
        perror("ttyname");
    }

int ttyslot(void);

    Purpose: Returns the index of the control terminal for the current process.
    Usage: To get the slot number for the terminal, useful for managing terminal sessions.
    Example:

    c

        int slot = ttyslot();
        printf("Terminal slot: %d\n", slot);

sys/ioctl.h Functions

    int ioctl(int fd, unsigned long request, ...);
        Purpose: General-purpose control for I/O devices, often used to control terminal settings.
        Usage: Modify terminal properties like window size, non-blocking mode, etc.
        Example: Setting the terminal to non-blocking mode.

        c

        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

termios.h Functions

    int tcsetattr(int fd, int optional_actions, const struct termios *termios_p);
        Purpose: Sets the parameters associated with the terminal.
        Usage: Change terminal attributes, such as disabling canonical mode and echo.
        Example:

        c

    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

int tcgetattr(int fd, struct termios *termios_p);

    Purpose: Gets the parameters associated with the terminal.
    Usage: Retrieve current terminal settings.
    Example:

    c

        struct termios term;
        if (tcgetattr(STDIN_FILENO, &term) == -1) {
            perror("tcgetattr");
        }

termcap.h Functions

    int tgetent(char *bp, const char *name);
        Purpose: Loads the terminal entry for the terminal type.
        Usage: Initialize the termcap library.
        Example:

        c

    char term_buffer[2048];
    if (tgetent(term_buffer, getenv("TERM")) != 1) {
        fprintf(stderr, "Could not access the termcap data base.\n");
    }

int tgetflag(char id[2]);

    Purpose: Gets the boolean value of a capability.
    Usage: Determine terminal capabilities.
    Example:

    c

    if (tgetflag("bs")) {
        printf("Terminal supports backspace.\n");
    }

int tgetnum(char id[2]);

    Purpose: Gets the numeric value of a capability.
    Usage: Retrieve numeric capabilities like the number of columns.
    Example:

    c

    int columns = tgetnum("co");
    printf("Number of columns: %d\n", columns);

char *tgetstr(char id[2], char **area);

    Purpose: Gets the string value of a capability.
    Usage: Retrieve string capabilities like the clear screen sequence.
    Example:

    c

    char *clear_screen = tgetstr("cl", NULL);
    tputs(clear_screen, 1, putchar);

char *tgoto(const char *cap, int col, int row);

    Purpose: Returns a cursor motion string for the given column and row.
    Usage: Move the cursor to a specified position.
    Example:

    c

    char *move_cursor = tgoto(tgetstr("cm", NULL), 10, 5);
    tputs(move_cursor, 1, putchar);

int tputs(const char *str, int affcnt, int (*putc)(int));

    Purpose: Puts the string on the terminal.
    Usage: Output a terminal control string.
    Example:

    c

tputs(clear_screen, 1, putchar);












 * 		EXETERNAL FUNCTIONS


// readline library
// Management: Commands like history, !number, and !! can interact with the history
char *readline(const char *prompt);
void rl_clear_history(void);
int rl_on_new_line(void);
	> after handling signals or asynchronous events that affect the terminal
int rl_replace_line(const char *text, int clear_undo);
	> auto-completion
int rl_redisplay(void);
	> Redraws the prompt and input line.
int add_history(const char *string);

// unistd.h
int access(const char *pathname, int mode);

// sys/wait.h
pid_t wait(int *wstatus);
pid_t waitpid(pid_t pid, int *wstatus, int options);
pid_t wait3(int *wstatus, int options, struct rusage *rusage);
pid_t wait4(pid_t pid, int *wstatus, int options, struct rusage *rusage);

// signal.h
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
int sigemptyset(sigset_t *set);
int sigaddset(sigset_t *set, int signum);
int kill(pid_t pid, int sig);

// unistd.h
char *getcwd(char *buf, size_t size);
int chdir(const char *path);

// sys/stat.h
int stat(const char *pathname, struct stat *statbuf);
int lstat(const char *pathname, struct stat *statbuf);
int fstat(int fd, struct stat *statbuf);

// unistd.h
int unlink(const char *pathname);
int dup(int oldfd);

// dirent.h
DIR *opendir(const char *name);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp);

// unistd.h
int isatty(int fd);

// unistd.h
char *ttyname(int fd);
int ttyslot(void);

// sys/ioctl.h
int ioctl(int fd, unsigned long request, ...);

// stdlib.h
char *getenv(const char *name);

// termios.h
int tcsetattr(int fd, int optional_actions, const struct termios *termios_p);
int tcgetattr(int fd, struct termios *termios_p);

// termcap.h
int tgetent(char *bp, const char *name);
int tgetflag(char id[2]);
int tgetnum(char id[2]);
char *tgetstr(char id[2], char **area);
char *tgoto(const char *cap, int col, int row);
int tputs(const char *str, int affcnt, int (*putc)(int));
******************************************************************************/