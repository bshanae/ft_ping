#include "ft_ping.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#if IPv6
void ipv6_send_packet()
{
	char send_buffer[BUFFER_SIZE];
	ft_bzero(send_buffer, ICMP_LENGTH + DATA_LENGTH);

	struct icmp6_hdr *icmp_header = (struct icmp6_hdr *) send_buffer;
	icmp_header->icmp6_type = ICMP6_ECHO_REQUEST;
	icmp_header->icmp6_code = 0;
	icmp_header->icmp6_id = data.instance_id;
	icmp_header->icmp6_seq = data.sequence_id++;
	gettimeofday((struct timeval *) (icmp_header + 1), NULL);

	int sent = sendto(
		data.socket,
		send_buffer,
		ICMP_LENGTH + DATA_LENGTH,
		0,
		data.host_address->ai_addr,
		data.host_address->ai_addrlen
	);

	if (sent != ICMP_LENGTH + DATA_LENGTH)
		exit_with_error("Error sending packet");

	data.sent_count++;
}

void ipv6_process_packet(char *ptr, size_t length)
{
	if (length < 8)
		exit_with_error("Unexpected ICMP header length: %d", length);

	struct icmp6_hdr *icmp6 = (struct icmp6_hdr *)ptr;
	if (icmp6->icmp6_type == ICMP6_ECHO_REPLY)
	{
		if (icmp6->icmp6_id != data.instance_id)
			return;
		if (length < 16)
			exit_with_error("Unexpected ICMP header length: %d", length);

		struct timeval *time_send = (struct timeval *)(icmp6 + 1);

		struct timeval time_received;
		gettimeofday(&time_received, NULL);

		struct timeval time_diff;
		timeval_subtract(&time_diff, &time_received, time_send);

		double rtt = time_diff.tv_sec * 1000.0 + time_diff.tv_usec / 1000.0;

		printf(
			"%lu bytes from %s: seq=%u, rtt=%.3f ms\n",
			length,
			data.host_address_str,
			icmp6->icmp6_seq,
			rtt
		);
	}
	else if (data.flag_verbose)
	{
		printf(
			"  %ld bytes from %s: type = %d, code = %d\n",
			length,
			data.host_address_str,
			icmp6->icmp6_type,
			icmp6->icmp6_code
		);
	}
}
#endif