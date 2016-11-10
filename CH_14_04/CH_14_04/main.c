#include <avr/io.h>
#include <avr/interrupt.h>

int state = 0;

ISR(TIMER1_COMPA_vect)
{
	TCNT1 = 0;				// 자동으로 0으로 변하지 않는다
}

int main(void)
{
	DDRB = 0x20;				// PB5 핀을 출력으로 설정
	PORTB = 0x00;				// LED는 끈 상태에서 시작
	
	OCR1A = 0x7FFF;			// 비교일치 값 설정
	
	TCCR1B |= (1 << CS12);		// 분주비를 256으로 설정

	// 비교일치 인터럽트 발생 시 OC1A 핀의 출력을 반전
	TCCR1A |= (1 << COM1A0);
	
	TIMSK |= (1 << OCIE1A);		// 비교일치 A 인터럽트 허용
	
	sei();							// 전역적으로 인터럽트 허용
	
	while(1){ }
	return 0;
}