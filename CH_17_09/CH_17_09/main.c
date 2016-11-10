#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "UART1.h"
#include "I2C_RTC.h"

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

int count = 0;

uint8_t bcd_to_decimal(uint8_t bcd)	// BCD 형식 -> 이진수 형식
{
	return (bcd >> 4) * 10 + (bcd & 0x0F);
}

uint8_t decimal_to_bcd(uint8_t decimal)	// 이진수 형식 -> BCD 형식
{
	return ( ((decimal / 10) << 4) | (decimal % 10) );
}

void INIT_INT2(void)
{
	EIMSK |= (1 << INT2);			// INT2 인터럽트 활성화
	EICRA |= (1 << ISC21);			// 하강 에지에서 인터럽트 발생
	sei();							// 전역적으로 인터럽트 허용
}

ISR(INT2_vect)				// INT2 인터럽트 서비스 루틴
{
	printf("%d\r\n", ++count);
}

int main(void)
{
	uint8_t i;
	
	I2C_init();					// I2C 초기화
	UART1_init();				// UART 초기화
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	uint8_t address = 0x68;		// RTC 모듈의 I2C 주소
	
	INIT_INT2();				// INT2 인터럽트 설정
	
	// 초, 분, 시, 요일, 일, 월, 년
	// 2016년 1월 1일 월요일 12시 00분 00초
	uint8_t data[] = {00, 00, 12, 2, 1, 1, 16};
	
	// RTC 모듈에 시간 설정
	I2C_start();				// I2C 시작
	I2C_transmit(address << 1);		// I2C 주소 전송. 쓰기 모드
	// RTC에 데이터를 기록할 메모리 시작 주소 전송
	I2C_transmit(0);
	
	for(i = 0; i < 7; i++){
		I2C_transmit(decimal_to_bcd(data[i]));	// 시간 설정
	}
	I2C_transmit(0x10);			// 구형파 출력 설정

	I2C_stop();					// I2C 정지
	
	while(1){}
	return 0;
}