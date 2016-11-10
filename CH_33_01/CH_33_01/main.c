#include <avr/io.h>
#include <avr/boot.h>
#include <stdio.h>
#include "UART1.h"

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

int main(void)
{
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init();				// UART 통신 초기화
	
	printf("Extended Fuse : 0x%X\r\n",
	boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS));
	printf("High     Fuse : 0x%X\r\n",
	boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS));
	printf("Low      Fuse : 0x%X\r\n",
	boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS));
	
	while (1);
	return 0;
}