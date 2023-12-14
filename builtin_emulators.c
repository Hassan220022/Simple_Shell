#include "shell.h"

/**
 * my_exit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit last_status
 *         (0) if info.input_arr[0] != "exit"
 */
int my_exit(command_info *info)
{
	int exitcheck;

	if (info->input_arr[1]) /* If there is an exit arguement */
	{
		exitcheck = _erra_toi(info->input_arr[1]);
		if (exitcheck == -1)
		{
			info->last_status = 2;
			_print_error(info, "Illegal number: ");
			error_puts(info->input_arr[1]);
			error_putchar('\n');
			return (1);
		}
		info->exit_code = _erra_toi(info->input_arr[1]);
		return (-2);
	}
	info->exit_code = -1;
	return (-2);
}

/**
 * my_cd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int my_cd(command_info *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->input_arr[1])
	{
		dir = get_enviroment(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = get_enviroment(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_str_cmp(info->input_arr[1], "-") == 0)
	{
		if (!get_enviroment(info, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(get_enviroment(info, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = get_enviroment(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->input_arr[1]);
	if (chdir_ret == -1)
	{
		_print_error(info, "can't cd to ");
		error_puts(info->input_arr[1]), error_putchar('\n');
	}
	else
	{
		set_enviroment(info, "OLDPWD", get_enviroment(info, "PWD="));
		set_enviroment(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * my_help - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int my_help(command_info *info)
{
	char **arg_array;

	arg_array = info->input_arr;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}
