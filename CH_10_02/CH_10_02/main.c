#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART1.h"

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

void ADC_init(unsigned char channel)
{
	ADMUX |= (1 << REFS0); 		// AVCC를 기준 전압으로 선택
	
	ADCSRA |= 0x07;			// 분주비 설정
	ADCSRA |= (1 << ADEN);		// ADC 활성화
	ADCSRA |= (1 << ADFR);		// free running 모드

	ADMUX = ((ADMUX & 0xE0) | channel);	// 채널 선택
	ADCSRA |= (1 << ADSC);		// 변환 시작
}

int read_ADC(void)
{
	while(!(ADCSRA & (1 << ADIF)));	// 변환 종료 대기
	
	return ADC;				// 10비트 값을 반환
}

int main(void)
{
	int read;
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init();				// UART 통신 초기화
	ADC_init(0);				// AD 변환기 초기화
	
	DDRB = 0xFF;				// 포트 B를 출력으로 설정
	
	while(1)
	{
		read = read_ADC();			// 가변저항 값 읽기

		uint8_t pattern = 0;			// LED 제어값
		int LED_count = (read >> 7) + 1;	// 켜질 LED의 개수

		for(int i = 0; i < LED_count; i++){	// LED 제어값 생성
			pattern |= (0x01 << i);
		}

		PORTB = pattern;			// LED 켜기
		
		printf("Read : %d,\t%d LEDs are ON!\r\n", read, LED_count);
		
		_delay_ms(1000);
	}

	return 0;
}