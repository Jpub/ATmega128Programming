#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "UART_support.h"

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	UART0_init();				// UART0 초기화
	stdout = &OUTPUT;			// printf 사용 설정
	
	// 사용자 작성 부분
	unsigned int count = 0;
	while(1)
	{
		printf("%d\n\r", count++);
		_delay_ms(1000);
	}
	
	return 0;
}