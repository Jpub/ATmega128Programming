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
	// AD 변환기를 밴드갭 전압을 읽도록 초기화
	ADC_init(0B11110);
	
	while(1)
	{
		read = read_ADC();			// 밴드갭 전압 읽기

		float AVCC = (1.23 * 1023) / read;	// AVCC 전압 계산
		
		printf("AVCC = %f\r\n", AVCC);
		
		_delay_ms(1000);
	}

	return 0;
}