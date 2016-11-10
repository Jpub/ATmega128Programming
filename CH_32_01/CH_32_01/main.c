#define F_CPU 16000000L
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include "UART1.h"

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

int main(void)
{
	wdt_enable(WDTO_2S);			// 와치독 타이머 만료 시간을 2초로 설정

	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init();				// UART 통신 초기화

	printf("*** Initialization...\r\n");
	uint16_t count = 0;
	
	while (1)
	{
		count++;
		printf("Count : %d\r\n", count);
		_delay_ms(1000);
	}
	
	return 0;
}
