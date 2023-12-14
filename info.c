#include "shell.h"

/**
 * _clear_info - initializes command_info struct
 * @info: struct address
 */
void _clear_info(command_info *info)
{
	info->input_str = NULL;
	info->input_arr = NULL;
	info->cmd_path = NULL;
	info->param_count = 0;
}

/**
 * _set_info - initializes command_info struct
 * @info: struct address
 * @av: argument vector
 */
void _set_info(command_info *info, char **av)
{
	int i = 0;

	info->file_name = av[0];
	if (info->input_str)
	{
		info->input_arr = str_tow(info->input_str, " \t");
		if (!info->input_arr)
		{

			info->input_arr = malloc(sizeof(char *) * 2);
			if (info->input_arr)
			{
				info->input_arr[0] = _str_dup(info->input_str);
				info->input_arr[1] = NULL;
			}
		}
		for (i = 0; info->input_arr && info->input_arr[i]; i++)
			;
		info->param_count = i;

		tokenized_replace_alias(info);
		tokenized_replace_vars(info);
	}
}

/**
 * _free_info - frees command_info struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void _free_info(command_info *info, int all)
{
	f_free(info->input_arr);
	info->input_arr = NULL;
	info->cmd_path = NULL;
	if (all)
	{
		if (!info->cmd_chain_buf)
			free(info->input_str);
		if (info->local_env)
			freeList(&(info->local_env));
		if (info->cmd_history)
			freeList(&(info->cmd_history));
		if (info->alias_node)
			freeList(&(info->alias_node));
		f_free(info->custom_environ);
		info->custom_environ = NULL;
		b_free((void **)info->cmd_chain_buf);
		if (info->input_fd > 2)
			close(info->input_fd);
		_putchar(BUFFER_FLUSH);
	}
}
