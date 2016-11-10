#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "UART1.h"
#include "I2C_RTC.h"

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

uint8_t bcd_to_decimal(uint8_t bcd)	// BCD 형식 -> 이진수 형식
{
	return (bcd >> 4) * 10 + (bcd & 0x0F);
}

uint8_t decimal_to_bcd(uint8_t decimal)	// 이진수 형식 -> BCD 형식
{
	return ( ((decimal / 10) << 4) | (decimal % 10) );
}

int main(void)
{
	uint8_t i;
	
	I2C_init();					// I2C 초기화
	UART1_init();				// UART 초기화
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	uint8_t address = 0x68;		// RTC 모듈의 I2C 주소
	
	// 초, 분, 시, 요일, 일, 월, 년
	// 2016년 1월 1일 월요일 12시 00분 00초
	uint8_t data[] = {00, 00, 12, 2, 1, 1, 16};
	
	// RTC 모듈에 시간 설정
	I2C_start();				// I2C 시작
	I2C_transmit(address << 1);		// I2C 주소 전송. 쓰기 모드
	// RTC에 데이터를 기록할 메모리 시작 주소 전송
	I2C_transmit(0);
	
	printf("* Setting RTC module...\r\n");
	for(i = 0; i < 7; i++){
		printf(" %dth byte written...\r\n", i);
		
		I2C_transmit(decimal_to_bcd(data[i]));	// 시간 설정
	}

	I2C_stop();					// I2C 정지
	
	_delay_ms(2000);			// 2초 대기
	
	I2C_start();				// I2C 시작
	I2C_transmit(address << 1);		// I2C 주소 전송. 쓰기 모드
	// RTC에서 데이터를 읽어올 메모리 시작 주소 전송
	I2C_transmit(0);
	I2C_stop();					// I2C 정지
	
	I2C_start();				// I2C 읽기 모드로 다시 시작
	I2C_transmit( (address << 1) + 1 );	// I2C 주소 전송. 읽기 모드
	
	printf("* Time/Date Retrieval...\r\n");
	
	printf(" %d second\r\n", bcd_to_decimal(I2C_receive_ACK()));
	printf(" %d minute\r\n", bcd_to_decimal(I2C_receive_ACK()));
	printf(" %d hour\r\n", bcd_to_decimal(I2C_receive_ACK()));
	printf(" %d day of week\r\n", bcd_to_decimal(I2C_receive_ACK()));
	printf(" %d day\r\n", bcd_to_decimal(I2C_receive_ACK()));
	printf(" %d month\r\n", bcd_to_decimal(I2C_receive_ACK()));
	printf(" %d year\r\n", bcd_to_decimal(I2C_receive_NACK()));
	
	I2C_stop();					// I2C 정지

	while(1);
	return 0;
}