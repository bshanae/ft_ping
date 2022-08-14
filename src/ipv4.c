#include "ft_ping.h"

#include <stdio.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

void ipv4_send_packet()
{
	char send_buffer[BUFFER_SIZE];
	ft_bzero(send_buffer, ICMP_LENGTH + DATA_LENGTH);

	struct icmp *icmp = (struct icmp *) send_buffer;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = data.instance_id;
	icmp->icmp_seq = data.sequence_id++;
	gettimeofday((struct timeval *) icmp->icmp_data, NULL);

	icmp->icmp_cksum = compute_checksum((unsigned short *) icmp, ICMP_LENGTH + DATA_LENGTH);

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

void ipv4_process_packet(char *ptr, size_t length)
{
	struct ip *ip_header = (struct ip *) ptr;
	int ip_header_length = ip_header->ip_hl * 4;

	struct icmp *icmp = (struct icmp *) (ptr + ip_header_length);
	size_t icmp_header_length = length - ip_header_length;
	if (icmp_header_length < 8)
		exit_with_error("Unexpected ICMP header length: %d", icmp_header_length);

	if (icmp->icmp_type == ICMP_ECHOREPLY)
	{
		if (icmp->icmp_id != data.instance_id)
			return;
		if (icmp_header_length < 16)
			exit_with_error("Unexpected ICMP header length: %d", icmp_header_length);

		struct timeval *time_send = (struct timeval *) icmp->icmp_data;

		struct timeval time_received;
		gettimeofday(&time_received, NULL);

		struct timeval time_diff;
		timeval_subtract(&time_diff, &time_received, time_send);

		double rtt = time_diff.tv_sec * 1000.0 + time_diff.tv_usec / 1000.0;
		printf(
			"%lu bytes from %s: seq=%u, ttl=%d, rtt=%.3f ms\n",
			icmp_header_length,
			data.host_address_str,
			icmp->icmp_seq,
			ip_header->ip_ttl,
			rtt
		);

		data.received_count++;
		data.rtt_min = MIN(data.rtt_min, rtt);
		data.rtt_max = MAX(data.rtt_max, rtt);
		data.rtt_sum += rtt;
		data.rtt_sum_squared += rtt * rtt;
	}
	else if (data.flag_verbose)
	{
		printf(
			"  %lu bytes from %s: type = %d, code = %d\n",
			icmp_header_length,
			data.host_address_str,
			icmp->icmp_type, icmp->icmp_code
		);
	}
}