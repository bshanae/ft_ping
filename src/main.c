#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <sys/uio.h>
#include <errno.h>
#include <math.h>
#include <float.h>

#include "ft_ping.h"

struct ft_ping_data data;

void alarm_handler(int _)
{
#ifdef IPv6
	ipv6_send_packet();
#else
	ipv4_send_packet();
#endif

	alarm(1);
}

void interrupt_handler(int _)
{
	double percentage_lost = 0;
	double average = 0;
	double standard_deviation = 0;

	if (data.sent_count != 0)
		percentage_lost = 100 - ((data.received_count * 100.0) / data.sent_count);

	printf("--- %s ping statistics ---\n", data.send_address->ai_canonname ? data.send_address->ai_canonname : data.send_address_str);
	printf("%u packets transmitted, %u packets received, %.2f%% packet loss\n", data.sent_count, data.received_count, percentage_lost);
	if (data.sent_count != 0)
		average = data.rtt_sum / data.sent_count;
	if (data.received_count > 1)
		standard_deviation = sqrt((data.rtt_sum_squared - (data.rtt_sum * data.rtt_sum) / data.received_count) / (data.received_count - 1));
	if (data.received_count > 0)
		printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", data.rtt_min, average, data.rtt_max, standard_deviation);

	exit(0);
}

int main(int argc, char **argv)
{
	// Parse arguments.

	char *host_name;
	parse_arguments(argc, argv, &host_name);

	// Check root rights.

	if (geteuid() != 0)
		exit_with_error("usage error: Ping should be executed with root privileges.\n");

	// Calculate instance ID.

	data.instance_id = getpid() & 0xffff;

	// Calculate host address.

	struct addrinfo hints;
	ft_bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;
#ifdef IPv6
	hints.ai_family = AF_INET6;
#else
	hints.ai_family = AF_INET;
#endif
	hints.ai_socktype = SOCK_RAW;

	if (getaddrinfo(host_name, NULL, &hints, &data.send_address) != 0)
		exit_with_error("ping: unknown host\n", host_name);
	if (getaddrinfo(host_name, NULL, &hints, &data.receive_address) != 0)
		exit_with_error("ping: unknown host\n", host_name);

	// Calculate host address string.

#ifdef IPv6
	ipv6_ntop(data.send_address, data.send_address_str, sizeof(data.send_address_str));
#else
	ipv4_ntop(data.send_address, data.send_address_str, sizeof(data.send_address_str));
#endif

	// Set initial values for rtt statistics.

	data.rtt_min = FLT_MAX;
	data.rtt_max = FLT_MIN;

	// Print info about host.

	printf(
		"PING %s (%s): %d data bytes\n",
		data.send_address->ai_canonname ? data.send_address->ai_canonname : data.send_address_str,
		data.send_address_str,
		DATA_LENGTH
	);

	// Open socket.

#ifdef IPv6
	data.socket = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
#else
	data.socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
#endif

	if (data.socket == -1)
		exit_with_error("Can't open socket\n");

	// Set alarm handler and send first packet.

	signal(SIGALRM, alarm_handler);
	alarm_handler(0);

	// Set interrupt handler.

	signal(SIGINT, interrupt_handler);

	// Receive packets.

	char receive_buffer[BUFFER_SIZE];
	char control_buffer[BUFFER_SIZE];

	struct msghdr message_header;
	struct iovec io_buffer;

	ft_bzero(&message_header, sizeof(struct msghdr));
	io_buffer.iov_base = receive_buffer;
	io_buffer.iov_len = sizeof(receive_buffer);
	message_header.msg_name = data.receive_address->ai_addr;
	message_header.msg_namelen = data.receive_address->ai_addrlen;
	message_header.msg_iov = &io_buffer;
	message_header.msg_iovlen = 1;
	message_header.msg_control = control_buffer;
	message_header.msg_controllen = sizeof(control_buffer);

	while (1)
	{
		int received_count = recvmsg(data.socket, &message_header, 0);
		if (received_count == -1)
		{
			if (errno == EINTR)
				continue;
			else
				exit(1);
		}

#ifdef IPv6
		ipv6_process_packet(receive_buffer, received_count);
#else
		ipv4_process_packet(receive_buffer, received_count);
#endif
	}

	return 0;
}

