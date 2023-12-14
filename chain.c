#include "shell.h"

/**
 * ISchain - test if current char in buffer is a chain delimeter
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int ISchain(command_info *info, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		info->cmd_chain_type = COMMAND_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		info->cmd_chain_type = COMMAND_AND;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		info->cmd_chain_type = COMMAND_CHAIN;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * ch_chain - checks we should continue chaining based on last last_status
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void ch_chain(command_info *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (info->cmd_chain_type == COMMAND_AND)
	{
		if (info->last_status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (info->cmd_chain_type == COMMAND_OR)
	{
		if (!info->last_status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * tokenized_replace_alias - replaces an aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int tokenized_replace_alias(command_info *info)
{
	int i;
	linked_list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = _node_starts_with(info->alias_node, info->input_arr[0], '=');
		if (!node)
			return (0);
		free(info->input_arr[0]);
		p = _str_chr(node->str, '=');
		if (!p)
			return (0);
		p = _str_dup(p + 1);
		if (!p)
			return (0);
		info->input_arr[0] = p;
	}
	return (1);
}

/**
 * tokenized_replace_vars - replaces vars in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int tokenized_replace_vars(command_info *info)
{
	int i = 0;
	linked_list_t *node;

	for (i = 0; info->input_arr[i]; i++)
	{
		if (info->input_arr[i][0] != '$' || !info->input_arr[i][1])
			continue;

		if (!_str_cmp(info->input_arr[i], "$?"))
		{
			shalowcopy_replace_string(&(info->input_arr[i]),
									  _str_dup(convert_number_itoa(info->last_status, 10, 0)));
			continue;
		}
		if (!_str_cmp(info->input_arr[i], "$$"))
		{
			shalowcopy_replace_string(&(info->input_arr[i]),
									  _str_dup(convert_number_itoa(getpid(), 10, 0)));
			continue;
		}
		node = _node_starts_with(info->local_env, &info->input_arr[i][1], '=');
		if (node)
		{
			shalowcopy_replace_string(&(info->input_arr[i]),
									  _str_dup(_str_chr(node->str, '=') + 1));
			continue;
		}
		shalowcopy_replace_string(&info->input_arr[i], _str_dup(""));
	}
	return (0);
}

/**
 * shalowcopy_replace_string - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int shalowcopy_replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
