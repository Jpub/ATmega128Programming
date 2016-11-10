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
	
	// EEPROM에 쓸 데이터
	uint8_t dataByte = 127;			// 2^7 - 1
	uint16_t dataWord = 32767;		// 2^15 - 1
	uint32_t dataDWord = 2147483647;	// 2^31 - 1
	char dataString[] = "ABCDE";

	// EEPROM의 주소
	int addressByte = 0, addressWord = 10, addressDWord = 20, addressString = 30;

	UART1_init();				// UART 통신 초기화
	
	// 데이터 쓰기
	eeprom_update_byte ( (uint8_t *)addressByte, dataByte);
	eeprom_update_word ( (uint16_t *)addressWord, dataWord);
	eeprom_update_dword ( (uint32_t *)addressDWord, dataDWord);
	eeprom_update_block ( (void *)dataString, (void *)addressString, 5);
	
	// 데이터 읽기
	dataByte = eeprom_read_byte ( (uint8_t *)addressByte );
	dataWord = eeprom_read_word ( (uint16_t *)addressWord );
	dataDWord = eeprom_read_dword ( (uint32_t *)addressDWord );
	eeprom_read_block ( (void *)dataString, (void *)addressString, 5);
	
	// UART 통신으로 데이터 전송
	printf("Byte        : %d\r\n", dataByte);			// 1 바이트 데이터
	printf("Word        : %d\r\n", dataWord);		// 2 바이트 데이터
	printf("Double Word : %ld\r\n", dataDWord);		// 4 바이트 데이터
	printf("String      : %s\r\n", dataString);		// 문자열 데이터
	
	while(1);
	
	return 0;
}