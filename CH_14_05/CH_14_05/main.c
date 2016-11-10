#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "UART1.h"

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

ISR(TIMER1_CAPT_vect)
{
	int temp1 = TCNT1;
	int temp2 = ICR1;
	
	printf("Input Capture : %u\r\n", temp2);
	printf("Timer/Counter : %u\r\n\r\n", temp1);
}

int main(void)
{
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init();				// UART 통신 초기화
	
	TCCR1B |= (1 << CS12) | (1 << CS10);	// 분주비를 1024로 설정

	TIMSK |= (1 << TICIE1);		// 입력 캡처 인터럽트 허용
	
	sei();							// 전역적으로 인터럽트 허용
	
	while(1){ }
	return 0;
}