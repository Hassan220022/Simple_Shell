#include "shell.h"

/**
 * fadd_node - adds a node to the start of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
linked_list_t *fadd_node(linked_list_t **head, const char *str, int num)
{
	linked_list_t *new_head;

	if (!head)
		return (NULL);
	new_head = malloc(sizeof(linked_list_t));
	if (!new_head)
		return (NULL);
	_mem_set((void *)new_head, 0, sizeof(linked_list_t));
	new_head->num = num;
	if (str)
	{
		new_head->str = _str_dup(str);
		if (!new_head->str)
		{
			free(new_head);
			return (NULL);
		}
	}
	new_head->next = *head;
	*head = new_head;
	return (new_head);
}

/**
 * add_node_atend - adds a node to the end of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
linked_list_t *add_node_atend(linked_list_t **head, const char *str, int num)
{
	linked_list_t *new_node, *node;

	if (!head)
		return (NULL);

	node = *head;
	new_node = malloc(sizeof(linked_list_t));
	if (!new_node)
		return (NULL);
	_mem_set((void *)new_node, 0, sizeof(linked_list_t));
	new_node->num = num;
	if (str)
	{
		new_node->str = _str_dup(str);
		if (!new_node->str)
		{
			free(new_node);
			return (NULL);
		}
	}
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = new_node;
	}
	else
		*head = new_node;
	return (new_node);
}

/**
 * print_list_text - prints only the str element of a linked_list_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t print_list_text(const linked_list_t *h)
{
	size_t i = 0;

	while (h)
	{
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * delete_node_index - deletes node at given index
 * @head: address of pointer to first node
 * @index: index of node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int delete_node_index(linked_list_t **head, unsigned int index)
{
	linked_list_t *node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (!index)
	{
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return (1);
	}
	node = *head;
	while (node)
	{
		if (i == index)
		{
			prev_node->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}
		i++;
		prev_node = node;
		node = node->next;
	}
	return (0);
}

/**
 * freeList - frees all nodes of a list
 * @head_ptr: address of pointer to head node
 *
 * Return: void
 */
void freeList(linked_list_t **head_ptr)
{
	linked_list_t *node, *next_node, *head;

	if (!head_ptr || !*head_ptr)
		return;
	head = *head_ptr;
	node = head;
	while (node)
	{
		next_node = node->next;
		free(node->str);
		free(node);
		node = next_node;
	}
	*head_ptr = NULL;
}
