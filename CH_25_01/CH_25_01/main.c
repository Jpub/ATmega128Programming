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
	ADCSRA |= (1 << ADFR);		// 프리 러닝 모드

	ADMUX |= ((ADMUX & 0xE0) | channel);	// 채널 선택
	ADCSRA |= (1 << ADSC);		// 변환 시작
}

int read_ADC(void)
{
	while(!(ADCSRA & (1 << ADIF)));	// 변환 종료 대기
	
	return ADC;				// 10비트 값을 반환
}

int main(void)
{
	int read, temperature;
	float input_voltage;
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init();				// UART 통신 초기화
	ADC_init(3);				// AD 변환기 초기화. 채널 3 사용
	
	while(1)
	{
		read = read_ADC();			// ADC 값 읽기
		// 0에서 1023 사이의 값을 0V에서 5V 사이 값으로 변환한다.
		input_voltage = read * 5.0 / 1023.0;
		
		// 10mV에 1℃이므로 100을 곱해서 현재 온도를 얻는다.
		temperature = (int)(input_voltage * 100.0);
		
		printf("%d\r\n", temperature);
		
		_delay_ms(1000);			// 1초에 한 번 읽음
	}

	return 0;
}