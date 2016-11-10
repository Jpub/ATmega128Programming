#include <avr/io.h>
#include <avr/interrupt.h>

int state = 0;

ISR(TIMER1_OVF_vect)
{
	state = !state;				// LED 상태 반전
	if(state) PORTB = 0x01;			// LED 켜기
	else PORTB = 0x00;			// LED 끄기
}

int main(void)
{
	DDRB = 0x01;				// PB0 핀을 출력으로 설정
	PORTB = 0x00;				// LED는 끈 상태에서 시작
	
	TCCR1B |= (1 << CS12);		// 분주비를 256으로 설정

	TIMSK |= (1 << TOIE1);			// 오버플로 인터럽트 허용
	
	sei();							// 전역적으로 인터럽트 허용
	
	while(1){ }
	return 0;
}