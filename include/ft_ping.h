#ifndef FT_PING_H
#define FT_PING_H

#include <stdlib.h>
#include <stddef.h>
#include <sys/time.h>
#include <netinet/ip.h>

#define BUFFER_SIZE 128
#define ICMP_LENGTH 8
#define DATA_LENGTH 56

#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)

struct ft_ping_data
{
	int flag_verbose;

	int instance_id;
	int socket;
	struct addrinfo *send_address;
	char send_address_str[64];
	struct addrinfo *receive_address;
	char receive_address_str[64];
	int sequence_id;

	int sent_count;
	int received_count;
	double rtt_min;
	double rtt_max;
	double rtt_sum;
	double rtt_sum_squared;
};

extern struct ft_ping_data data;

void address_ntop(struct addrinfo *address, char *buffer, size_t buffer_size);
void send_packet();
void process_packet(char *ptr, size_t length);

void parse_arguments(int argc, char **argv, char **host_name);
void exit_with_error(char *format, ...);
void ft_bzero(void *str, size_t n);
void *ft_memset(void *str, int c, size_t n);
unsigned short compute_checksum(unsigned short *ptr, int len);
int timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y);

#endif