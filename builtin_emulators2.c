#include "shell.h"

/**
 * my_history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int my_history(command_info *info)
{
	print_linked_list(info->cmd_history);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(command_info *info, char *str)
{
	char *p, c;
	int ret;

	p = _str_chr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_index(&(info->alias_node),
							_get_index_node(info->alias_node, _node_starts_with(info->alias_node, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(command_info *info, char *str)
{
	char *p;

	p = _str_chr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(info, str));

	unset_alias(info, str);
	return (add_node_atend(&(info->alias_node), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(linked_list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _str_chr(node->str, '=');
		for (a = node->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * my_alias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int my_alias(command_info *info)
{
	int i = 0;
	char *p = NULL;
	linked_list_t *node = NULL;

	if (info->param_count == 1)
	{
		node = info->alias_node;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->input_arr[i]; i++)
	{
		p = _str_chr(info->input_arr[i], '=');
		if (p)
			set_alias(info, info->input_arr[i]);
		else
			print_alias(_node_starts_with(info->alias_node, info->input_arr[i], '='));
	}

	return (0);
}
