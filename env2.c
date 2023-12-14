#include "shell.h"

/**
 * _get_environ - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **_get_environ(command_info *info)
{
	if (!info->custom_environ || info->env_modified)
	{
		info->custom_environ = linked_list_to_strings(info->local_env);
		info->env_modified = 0;
	}

	return (info->custom_environ);
}

/**
 * unset_enviroment - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int unset_enviroment(command_info *info, char *var)
{
	linked_list_t *node = info->local_env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = _starts_with(node->str, var);
		if (p && *p == '=')
		{
			info->env_modified = delete_node_index(&(info->local_env), i);
			i = 0;
			node = info->local_env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_modified);
}

/**
 * set_enviroment - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int set_enviroment(command_info *info, char *var, char *value)
{
	char *buf = NULL;
	linked_list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_str_len(var) + _str_len(value) + 2);
	if (!buf)
		return (1);
	_str_cpy(buf, var);
	_str_cat(buf, "=");
	_str_cat(buf, value);
	node = info->local_env;
	while (node)
	{
		p = _starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_modified = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_atend(&(info->local_env), buf, 0);
	free(buf);
	info->env_modified = 1;
	return (0);
}
