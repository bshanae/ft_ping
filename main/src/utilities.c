#include "ft_ping.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void exit_with_error(char *format, ...)
{
	va_list args;
	va_start(args, format);

	vprintf(format, args);

	va_end(args);

	exit(1);
}

void ft_bzero(void *str, size_t n)
{
	ft_memset(str, 0, n);
}

void *ft_memset(void *str, int c, size_t n)
{
	unsigned char *uc_str;
	unsigned long *ul_str;
	unsigned long ul_c;

	uc_str = (unsigned char *) str;
	while (n % 8)
	{
		*(uc_str++) = (unsigned char) c;
		n--;
	}

	ul_c = 0x0101010101010101 * (unsigned char) c;
	ul_str = (unsigned long *) uc_str;
	while (n > 0)
	{
		*(ul_str++) = ul_c;
		n -= 8;
	}

	return str;
}

unsigned short compute_checksum(unsigned short *ptr, int len)
{
	int left = len;
	int sum = 0;
	unsigned short result = 0;

	while (left > 1)
	{
		sum += *ptr++;
		left -= 2;
	}

	if (left == 1)
	{
		*(unsigned char *) (&result) = *(unsigned char *)ptr;
		sum += result;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	result = ~sum;
	return (result);
}

// https://www.gnu.org/software/libc/manual/html_node/Calculating-Elapsed-Time.html
int timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y)
{
	/* Perform the carry for the later subtraction by updating y. */
	if (x->tv_usec < y->tv_usec)
	{
		int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
		y->tv_usec -= 1000000 * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_usec - y->tv_usec > 1000000)
	{
		int nsec = (x->tv_usec - y->tv_usec) / 1000000;
		y->tv_usec += 1000000 * nsec;
		y->tv_sec -= nsec;
	}

	/* Compute the time remaining to wait.
	   tv_usec is certainly positive. */
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_usec = x->tv_usec - y->tv_usec;

	/* Return 1 if result is negative. */
	return x->tv_sec < y->tv_sec;
}
