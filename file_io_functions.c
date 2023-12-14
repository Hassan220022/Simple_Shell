#include "shell.h"

/**
 * get_cmd_history_file - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containg history file
 */

char *get_cmd_history_file(command_info *info)
{
	char *buf, *dir;

	dir = get_enviroment(info, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_str_len(dir) + _str_len(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_str_cpy(buf, dir);
	_str_cat(buf, "/");
	_str_cat(buf, HIST_FILE);
	return (buf);
}

/**
 * write_cmd_history - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_cmd_history(command_info *info)
{
	ssize_t fd;
	char *filename = get_cmd_history_file(info);
	linked_list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->cmd_history; node; node = node->next)
	{
		_puts_fd(node->str, fd);
		_put_fd('\n', fd);
	}
	_put_fd(BUFFER_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_cmd_history - reads history from file
 * @info: the parameter struct
 *
 * Return: history_counter on success, 0 otherwise
 */
int read_cmd_history(command_info *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_cmd_history_file(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			build_cmd_history_list(info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_cmd_history_list(info, buf + last, linecount++);
	free(buf);
	info->history_counter = linecount;
	while (info->history_counter-- >= HIST_MAX)
		delete_node_index(&(info->cmd_history), 0);
	renumber_history(info);
	return (info->history_counter);
}

/**
 * build_cmd_history_list - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int build_cmd_history_list(command_info *info, char *buf, int linecount)
{
	linked_list_t *node = NULL;

	if (info->cmd_history)
		node = info->cmd_history;
	add_node_atend(&node, buf, linecount);

	if (!info->cmd_history)
		info->cmd_history = node;
	return (0);
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int renumber_history(command_info *info)
{
	linked_list_t *node = info->cmd_history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->history_counter = i);
}
