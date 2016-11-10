#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int state = 0;			// 현재 LED의 상태

ISR(INT0_vect)
{
	state = (state + 1) % 2;			// LED 상태 전환
}

void INIT_PORT(void)
{
	DDRB = 0x01;				// PB0 핀을 출력으로 설정
	PORTB = 0x00;				// LED는 꺼진 상태에서 시작
	
	DDRD = 0x00;				// PD0 핀을 입력으로 설정
	PORTD = 0x01;			// PD0 핀의 풀업 저항 사용
}

void INIT_INT0(void)
{
	EIMSK |= (1 << INT0);			// INT0 인터럽트 활성화
	EICRA |= (1 << ISC01);			// 하강 에지에서 인터럽트 발생
	sei();							// 전역적으로 인터럽트 허용
}

int main(void)
{
	INIT_PORT();				// 포트 설정
	INIT_INT0();				// INT0 인터럽트 설정
	
	while(1){
		if(state == 1){
			PORTB = 0x01;
		}
		else{
			PORTB = 0x00;
		}
	}

	return 0;
}