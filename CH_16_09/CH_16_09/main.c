#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>

#include "UART1.h"
#include "EEPROM_25LC010.h"

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

int main(void)
{
	uint8_t i;
	
	SPI_Init();					// SPI 초기화
	UART1_init();				// UART 초기화
	
	stdout = &OUTPUT;
	stdin = &INPUT;

	for(i = 0; i < 128; i++)			// EEPROM에 쓰기
	EEPROM_writeByte(i, i);
	
	for(i = 0; i < 128; i++){			// EEPROM에서 읽기
		printf("%d\n\r", EEPROM_readByte(i));
	}
	
	while(1);
	return 0;
}