#include <avr/io.h>
#include <avr/eeprom.h>
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
	
	uint8_t *p1, *p2;
	p1 = 0;
	// p1 번지에 uint8_t 데이터 형의 바이트 수를 더한 값 = 1
	p2 = p1 + 1;
	
	printf("** 8 bit address...\r\n");
	printf("P1     : %d\r\n", (uint16_t)p1);
	printf("P1 + 1 : %d\r\n", (uint16_t)p2);
	
	uint16_t *p3, *p4;
	p3 = 0;
	// p3 번지에 uint16_t 데이터 형의 바이트 수를 더한 값 = 2
	p4 = p3 + 1;
	
	printf("** 16 bit address...\r\n");
	printf("P3     : %d\r\n", (uint16_t)p3);
	printf("P3 + 1 : %d\r\n", (uint16_t)p4);
	
	while(1);
	
	return 0;
}