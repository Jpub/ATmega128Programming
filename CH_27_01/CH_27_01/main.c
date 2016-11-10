#include <avr/io.h>
#include <stdio.h>
#include "UART0.h"
#include "UART1.h"

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

int main(void)
{
	uint8_t data;
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART0_init();				// UART0 통신 초기화
	UART1_init();				// UART1 통신 초기화
	
	while (1)
	{
		data = UART0_receive();
		
		printf("%c", data);
	}

	return 0;
}