#include "shell.h"

/**
 * linked_list_lenth - determines length of linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t linked_list_lenth(const linked_list_t *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * linked_list_to_strings - returns an array of strings of the list->str
 * @head: pointer to first node
 *
 * Return: array of strings
 */
char **linked_list_to_strings(linked_list_t *head)
{
	linked_list_t *node = head;
	size_t i = linked_list_lenth(head), j;
	char **strs;
	char *str;

	if (!head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(_str_len(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = _str_cpy(str, node->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}

/**
 * print_linked_list - prints all elements of a linked_list_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t print_linked_list(const linked_list_t *h)
{
	size_t i = 0;

	while (h)
	{
		_puts(convert_number_itoa(h->num, 10, 0));
		_putchar(':');
		_putchar(' ');
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * _node_starts_with - returns node whose string starts with prefix
 * @node: pointer to list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match node or null
 */
linked_list_t *_node_starts_with(linked_list_t *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = _starts_with(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node);
		node = node->next;
	}
	return (NULL);
}

/**
 * _get_index_node - gets the index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: index of node or -1
 */
ssize_t _get_index_node(linked_list_t *head, linked_list_t *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node)
			return (i);
		head = head->next;
		i++;
	}
	return (-1);
}
