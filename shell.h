#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024
#define BUFFER_FLUSH -1

/* for command chaining */
#define COMMAND_NORM 0
#define COMMAND_OR 1
#define COMMAND_AND 2
#define COMMAND_CHAIN 3

/* for convert_number_itoa() */
#define CONV_LOWERCASE 1
#define CONV_UNSIGNED 2

/* 1 if using system getline() */
#define USE_GET_LINE 0
#define USE_GET_STRTOK 0

#define HIST_FILE ".simple_shell_history"
#define HIST_MAX 4096

extern char **environ;

/**
 * struct linked_list_node - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct linked_list_node
{
	int num;
	char *str;
	struct linked_list_node *next;
} linked_list_t;

/**
 *struct command_info - contains pseudo-arguements to pass into a function,
 *					allowing uniform prototype for function pointer struct
 *@input_str: a string generated from getline containing arguements
 *@input_arr: an array of strings generated from input_str
 *@cmd_path: a string path for the current command
 *@param_count: the argument count
 *@line_counter: the error count
 *@exit_code: the error code for exit()s
 *@line_counter_flag: if on count this line of input
 *@file_name: the program filename
 *@local_env: linked list local copy of environ
 *@custom_environ: custom modified copy of environ from LL local_env
 *@cmd_history: the history node
 *@alias_node: the alias node
 *@env_modified: on if environ was changed
 *@last_status: the return last_status of the last exec'd command
 *@cmd_chain_buf: address of pointer to cmd_chain_buf, on if chaining
 *@cmd_chain_type: CMD_type ||, &&, ;
 *@input_fd: the fd from which to read line input
 *@history_counter: the history line number count
 */
typedef struct command_info
{
	char *input_str;
	char **input_arr;
	char *cmd_path;
	int param_count;
	unsigned int line_counter;
	int exit_code;
	int line_counter_flag;
	char *file_name;
	linked_list_t *local_env;
	linked_list_t *cmd_history;
	linked_list_t *alias_node;
	char **custom_environ;
	int env_modified;
	int last_status;

	char **cmd_chain_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_chain_type;	  /* CMD_type ||, &&, ; */
	int input_fd;
	int history_counter;
} command_info;

#define COMMAND_INFO_INIT                                                       \
	{                                                                           \
		NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
			0, 0, 0                                                             \
	}

/**
 *struct command - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct command
{
	char *type;
	int (*func)(command_info *);
} command_table;

/* hsh.c */
int main_shell_loop_hsh(command_info *, char **);
int _find_built_in(command_info *);
void _find_cmd(command_info *);
void _fork_cmd(command_info *);

/* path.c */
int Is_cmd(command_info *, char *);
char *duplicates_chars(char *, int, int);
char *find_cmd_path(command_info *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* err_string_functions.c */
void error_puts(char *);
int error_putchar(char);
int _put_fd(char c, int fd);
int _puts_fd(char *str, int fd);

/* string_functions.c */
int _str_len(char *);
int _str_cmp(char *, char *);
char *_starts_with(const char *, const char *);
char *_str_cat(char *, char *);

/* string_functions2.c */
char *_str_cpy(char *, char *);
char *_str_dup(const char *);
void _puts(char *);
int _putchar(char);

/* string_functions3.c */
char *_str_n_cpy(char *, char *, int);
char *_str_n_cat(char *, char *, int);
char *_str_chr(char *, char);

/* string_functions4.c */
char **str_tow(char *, char *);
char **str_tow_2(char *, char);

/* memory_functions */
char *_mem_set(char *, char, unsigned int);
void f_free(char **);
void *_real_loc(void *, unsigned int, unsigned int);

/* memory_functions2.c */
int b_free(void **);

/* more_functions.c */
int active(command_info *);
int is_delimiter(char, char *);
int is_alphabetical(int);
int _a_toi(char *);

/* more_functions2.c */
int _erra_toi(char *);
void _print_error(command_info *, char *);
int print_decimal(int, int);
char *convert_number_itoa(long int, int, int);
void remove_com(char *);

/* builtin_emulators.c */
int my_exit(command_info *);
int my_cd(command_info *);
int my_help(command_info *);

/* builtin_emulators2.c */
int my_history(command_info *);
int my_alias(command_info *);

/* getline.c module */
ssize_t getInput(command_info *);
int get_line(command_info *, char **, size_t *);
void sigint_Handler(int);

/* info.c module */
void _clear_info(command_info *);
void _set_info(command_info *, char **);
void _free_info(command_info *, int);

/* local_env.c module */
char *get_enviroment(command_info *, const char *);
int my_enviroment(command_info *);
int my_set_enviroment(command_info *);
int my_unset_enviroment(command_info *);
int populate_enviroment_list(command_info *);

/* env2.c module */
char **_get_environ(command_info *);
int unset_enviroment(command_info *, char *);
int set_enviroment(command_info *, char *, char *);

/* file_io_functions.c */
char *get_cmd_history_file(command_info *info);
int write_cmd_history(command_info *info);
int read_cmd_history(command_info *info);
int build_cmd_history_list(command_info *info, char *buf, int linecount);
int renumber_history(command_info *info);

/* linked_list_node.c module */
linked_list_t *fadd_node(linked_list_t **, const char *, int);
linked_list_t *add_node_atend(linked_list_t **, const char *, int);
size_t print_list_text(const linked_list_t *);
int delete_node_index(linked_list_t **, unsigned int);
void freeList(linked_list_t **);

/* liststr2.c module */
size_t linked_list_lenth(const linked_list_t *);
char **linked_list_to_strings(linked_list_t *);
size_t print_linked_list(const linked_list_t *);
linked_list_t *_node_starts_with(linked_list_t *, char *, char);
ssize_t _get_index_node(linked_list_t *, linked_list_t *);

/* chain.c */
int ISchain(command_info *, char *, size_t *);
void ch_chain(command_info *, char *, size_t *, size_t, size_t);
int tokenized_replace_alias(command_info *);
int tokenized_replace_vars(command_info *);
int shalowcopy_replace_string(char **, char *);

#endif
