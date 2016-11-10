#define F_CPU 16000000L
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "UART1.h"

// ‘bit’ 위치의 비트를 1 또는 0으로 설정하기 위한 매크로 함수
#define set_bit(bit) 	( PORTB |= _BV(bit) )
#define clear_bit(bit) 	( PORTB &= ~_BV(bit) )

// ATmega128의 포트 B에 연결된 위치
#define PARALLEL_LOAD			0
#define CLK					1
#define SERIAL_OUTPUT			2

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

int main(void)
{
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init();				// UART 통신 초기화
	
	// 병렬 입력과 클록 핀을 출력으로 설정
	DDRB |= _BV(PARALLEL_LOAD) | _BV(CLK);
	
	while (1)
	{
		// 저장 단계
		clear_bit(PARALLEL_LOAD);
		_delay_ms(5);
		
		set_bit(PARALLEL_LOAD);
		_delay_ms(5);

		// 직렬 출력 단계
		uint8_t button;
		for(int i = 0; i < 8; i++){
			button = (PINB & _BV(SERIAL_OUTPUT)) >> SERIAL_OUTPUT;
			printf("%c ", (button == 1) ? 'O' : 'X');

			// 상승 에지에서 직렬로 1비트씩 입력
			clear_bit(CLK);
			_delay_ms(1);
			set_bit(CLK);
			_delay_ms(1);
		}
		printf("\r\n");
		
		_delay_ms(1000);			// 1초에 한 번 버튼 검사
	}
	
	return 0;
}