#include <avr/io.h>
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

	DDRB |= 0x01;				// 릴레이 제어핀을 출력으로 설정
	char data;

	while (1)
	{
		scanf("%c", &data);			// UART 통신을 통한 제어 데이터 수신
		
		printf(">> You typed \n", data);
		
		if(data == 'o' || data == 'O'){
			PORTB |= 0x01;			// 릴레이 ON
			printf("** Switched ON !!\r\n");
		}
		else{
			PORTB &= 0xFE;			// 릴레이 OFF
			printf("** Switched OFF !!\r\n");
		}
	}
	
	return 0;
}