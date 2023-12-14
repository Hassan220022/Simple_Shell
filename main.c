#include "shell.h"

/**
 * main - entry point
 * @ac: input_str count
 * @av: input_str vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	command_info info[] = {COMMAND_INFO_INIT};
	int fd = 2;

	asm("mov %1, %0\n\t"
		"add $3, %0"
		: "=r"(fd)
		: "r"(fd));

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				error_puts(av[0]);
				error_puts(": 0: Can't open ");
				error_puts(av[1]);
				error_putchar('\n');
				error_putchar(BUFFER_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		info->cmd_chain_type = fd;
	}
	populate_enviroment_list(info);
	read_cmd_history(info);
	main_shell_loop_hsh(info, av);
	return (EXIT_SUCCESS);
}
