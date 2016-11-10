#include <avr/io.h>
#include <avr/interrupt.h>

int state0 = 0, state1 = 0, state2 = 0;

ISR(TIMER1_COMPA_vect)
{
	state2 = !state2;			// PB2 핀의 LED 제어
	if(state2) PORTB |= 0x04;
	else PORTB &= ~0x04;
}

ISR(TIMER1_COMPB_vect)
{
	state2 = !state2;			// PB2 핀의 LED 제어
	if(state2) PORTB |= 0x04;
	else PORTB &= ~0x04;
	
	state1 = !state1;			// PB1 핀의 LED 제어
	if(state1) PORTB |= 0x02;
	else PORTB &= ~0x02;
}

ISR(TIMER1_COMPC_vect)
{
	state2 = !state2;			// PB2 핀의 LED 제어
	if(state2) PORTB |= 0x04;
	else PORTB &= ~0x04;
}

ISR(TIMER1_OVF_vect)
{
	state2 = !state2;			// PB2 핀의 LED 제어
	if(state2) PORTB |= 0x04;
	else PORTB &= ~0x04;
	
	state1 = !state1;			// PB2 핀의 LED 제어
	if(state1) PORTB |= 0x02;
	else PORTB &= ~0x02;
	
	state0 = !state0;			// PB0 핀의 LED 제어
	if(state0) PORTB |= 0x01;
	else PORTB &= ~0x01;
}

int main(void)
{
	DDRB = 0x07;				// PB0~PB2 핀을 출력으로 설정
	PORTB = 0x00;				// LED는 끈 상태에서 시작
	
	OCR1A = 0x3FFF;			// 비교일치 A (1/4초 간격)
	OCR1B = 0x7FFF;			// 비교일치 B (2/4초 간격)
	OCR1C = 0xBFFF;			// 비교일치 C (3/4초 간격)
	
	TCCR1B |= (1 << CS12);		// 분주비를 256으로 설정

	// 비교일치 A & B, 오버플로 인터럽트 허용
	TIMSK |= (1 << OCIE1A) | (1 << OCIE1B) | (1 << TOIE1);
	// 비교일치 C 인터럽트 허용
	ETIMSK |= (1 << OCIE1C);
	
	sei();							// 전역적으로 인터럽트 허용
	
	while(1){ }
	return 0;
}