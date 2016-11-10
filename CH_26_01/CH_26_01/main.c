#include <avr/io.h>
#include <stdio.h>
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
	
	UART1_init();				// UART 통신 초기화

	while (1)
	{
		scanf("%c", &data);			// 문자 단위 데이터 수신
		printf("%c", data);			// 문자 단위 데이터 송신
	}
	
	return 0;
}