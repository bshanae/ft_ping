#include "ft_ping.h"
#include <stdio.h>

void parse_arguments(int argc, char **argv, char **host_name)
{
	if (argc < 2)
		exit_with_error("usage error: Destination address required\n");

	*host_name = NULL;

	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
				case 'v':
					data.flag_verbose = 1;
					break;

				case 'h':
					printf("Usage: ping [OPTION...] HOST ...\n");
					printf("Send ICMP ECHO_REQUEST packets to network hosts.\n");
					printf("Options:\n");
					printf("  -h	Show this menu.\n");
					printf("  -v	Verbose mode.\n");
					exit(0);
					break;

				default:
					exit_with_error("usage error: Unknown option '%s'\n", argv[i]);
					break;
			}
		}
		else
		{
			*host_name = argv[i];
		}
	}

	if (host_name == NULL)
		exit_with_error("usage error: Missing host operand\n");
}