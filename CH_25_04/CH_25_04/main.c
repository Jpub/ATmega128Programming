#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART1.h"

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

#define  PRESCALER 1024 		// 분주비

void Timer_init(void)
{
	// 16비트 타이머 1번 타이머/카운터 분주비를 1024로 설정
	TCCR1B |= (1 << CS12) | (1 << CS10);
}

uint8_t measure_distance(void)
{
	// 트리거 핀으로 펄스 출력
	PORTB &= ~(1 << PB1);			// LOW 값 출력
	_delay_us(1);
	PORTB |= (1 << PB1);			// HIGH 값 출력
	_delay_us(10);				// 10 마이크로초 대기
	PORTB &= ~(1 << PB1);			// LOW 값 출력
	
	// 에코 핀이 HIGH가 될 때까지 대기
	TCNT1 = 0;
	while(!(PINB & 0x01))
	if(TCNT1 > 65000) return 0;		// 장애물이 없는 경우

	// 에코 핀이 LOW가 될 때까지의 시간 측정
	TCNT1 = 0;				// 카운터를 0으로 초기화
	while(PINB & 0x01){
		if (TCNT1 > 650000){			// 측정 불가능
			TCNT1 = 0;
			break;
		}
	}

	// 에코 핀의 펄스 폭을 마이크로초 단위로 계산
	double pulse_width = 1000000.0 * TCNT1 * PRESCALER / F_CPU;
	
	return pulse_width / 58;		// 센티미터 단위 거리 반환
}

int main(void)
{
	uint8_t distance;
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init();				// UART 통신 초기화
	
	DDRB |= 0x02;				// 트리거 핀 출력으로 설정
	DDRB &= 0xFE;			// 에코 핀 입력으로 설정
	
	Timer_init();				// 1번 타이머/카운터 활성화
	
	while(1)
	{
		distance = measure_distance();	// 거리 측정
		
		printf("Distance : %d cm\r\n", distance);
		
		_delay_ms(1000);			// 1초에 한 번 읽음
	}

	return 0;
}