#include "shell.h"

/**
 * hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int main_shell_loop_hsh(command_info *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		_clear_info(info);
		if (active(info))
			_puts("$ ");
		error_putchar(BUFFER_FLUSH);
		r = getInput(info);
		if (r != -1)
		{
			_set_info(info, av);
			builtin_ret = _find_built_in(info);
			if (builtin_ret == -1)
				_find_cmd(info);
		}
		else if (active(info))
			_putchar('\n');
		_free_info(info, 0);
	}
	write_cmd_history(info);
	_free_info(info, 1);
	if (!active(info) && info->last_status)
		exit(info->last_status);
	if (builtin_ret == -2)
	{
		if (info->exit_code == -1)
			exit(info->last_status);
		exit(info->exit_code);
	}
	return (builtin_ret);
}

/**
 * _find_built_in - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int _find_built_in(command_info *info)
{
	int i, built_in_ret = -1;
	command_table builtintbl[] = {
		{"exit", my_exit},
		{"env", my_enviroment},
		{"help", my_help},
		{"history", my_history},
		{"setenv", my_set_enviroment},
		{"unsetenv", my_unset_enviroment},
		{"cd", my_cd},
		{"alias", my_alias},
		{NULL, NULL}};

	for (i = 0; builtintbl[i].type; i++)
		if (_str_cmp(info->input_arr[0], builtintbl[i].type) == 0)
		{
			info->line_counter++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * _find_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void _find_cmd(command_info *info)
{
	char *path = NULL;
	int i, k;

	info->cmd_path = info->input_arr[0];
	if (info->line_counter_flag == 1)
	{
		info->line_counter++;
		info->line_counter_flag = 0;
	}
	for (i = 0, k = 0; info->input_str[i]; i++)
		if (!is_delimiter(info->input_str[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_cmd_path(info, get_enviroment(info, "PATH="), info->input_arr[0]);
	if (path)
	{
		info->cmd_path = path;
		_fork_cmd(info);
	}
	else
	{
		if ((active(info) || get_enviroment(info, "PATH=") || info->input_arr[0][0] == '/') && Is_cmd(info, info->input_arr[0]))
			_fork_cmd(info);
		else if (*(info->input_str) != '\n')
		{
			info->last_status = 127;
			_print_error(info, "not found\n");
		}
	}
}

/**
 * _fork_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void _fork_cmd(command_info *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->cmd_path, info->input_arr, _get_environ(info)) == -1)
		{
			_free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->last_status));
		if (WIFEXITED(info->last_status))
		{
			info->last_status = WEXITSTATUS(info->last_status);
			if (info->last_status == 126)
				_print_error(info, "Permission denied\n");
		}
	}
}
