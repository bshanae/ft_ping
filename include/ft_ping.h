#ifndef FT_PING_H
#define FT_PING_H

#include <stddef.h>
#include <sys/time.h>
#include <netinet/ip.h>

#define BUFFER_SIZE 128
#define ICMP_LENGTH 8
#define DATA_LENGTH 56

#define FT_PING_IPV6 0

#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)

struct ft_ping_data
{
	int flag_verbose;

	int instance_id;
	int socket;
	struct addrinfo *host_address;
	int sequence_id;

	int sent_count;
	int received_count;
	double rtt_min;
	double rtt_max;
	double rtt_sum;
	double rtt_sum_squared;

#if !FT_PING_IPV6
	char host_address_str[INET_ADDRSTRLEN];
#else
	char host_address_str[INET6_ADDRSTRLEN];
#endif
};

extern struct ft_ping_data data;

void ipv4_send_packet();
void ipv4_process_packet(char *ptr, size_t length);

void exit_with_error(char *format, ...);
void ft_bzero(void *str, size_t n);
void *ft_memset(void *str, int c, size_t n);
unsigned short compute_checksum(unsigned short *ptr, int len);
int timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y);

#endif