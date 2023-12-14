#include "shell.h"

/**
 * my_enviroment - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int my_enviroment(command_info *info)
{
	print_list_text(info->local_env);
	return (0);
}

/**
 * get_enviroment - gets the value of an environ variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: local_env var name
 *
 * Return: the value
 */
char *get_enviroment(command_info *info, const char *name)
{
	linked_list_t *node = info->local_env;
	char *p;

	while (node)
	{
		p = _starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * my_set_enviroment - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int my_set_enviroment(command_info *info)
{
	if (info->param_count != 3)
	{
		error_puts("Incorrect number of arguements\n");
		return (1);
	}
	if (set_enviroment(info, info->input_arr[1], info->input_arr[2]))
		return (0);
	return (1);
}

/**
 * my_unset_enviroment - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int my_unset_enviroment(command_info *info)
{
	int i;

	if (info->param_count == 1)
	{
		error_puts("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->param_count; i++)
		unset_enviroment(info, info->input_arr[i]);

	return (0);
}

/**
 * populate_enviroment_list - populates local_env linked list
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int populate_enviroment_list(command_info *info)
{
	linked_list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_atend(&node, environ[i], 0);
	info->local_env = node;
	return (0);
}
