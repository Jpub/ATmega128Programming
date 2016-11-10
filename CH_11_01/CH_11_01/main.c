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

void Comparator_init(void)
{
	ACSR |= (1 << ACBG);			// 내부 밴드갭 전압을 양의 입력으로 설정
}

int main(void)
{
	int read, state;
	float voltage;
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init();				// UART 통신 초기화
	ADC_init(0);				// AD 변환기 초기화
	Comparator_init();			// 비교기 초기화
	
	DDRB |= 0x01;				// LED 연결 핀을 출력으로 설정
	
	while(1)
	{
		read = read_ADC();			// 가변저항 값 읽기
		voltage = read * 5.0 / 1024;		// 실제 전압 값으로 변환

		uint8_t mask = (1 << ACO);
		if((ACSR & mask) == mask){		// 가변저항 값이 1.23V보다 낮은 경우
			state = 1;
			PORTB |= 0x01;			// LED 켜기
		}
		else{
			state = 0;
			PORTB &= ~0x01;			// LED 끄기
		}
		
		printf("Voltage : %f,\tLED is %s!!\r\n",
		voltage,
		(state == 1) ? "ON" : "OFF"
		);
		
		_delay_ms(1000);
	}

	return 0;
}