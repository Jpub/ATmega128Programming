#include <avr/io.h>
#include <avr/interrupt.h>

int count = 0;				// 오버플로가 발생한 횟수
int state = 0;				// LED 점멸 상태

ISR(TIMER0_OVF_vect)
{
	count++;
	if(count == 32){			// 오버플로 32회 발생 = 0.5초 경과
		count = 0;				// 카운터 초기화
		state = !state;			// LED 상태 반전
		if(state) PORTB = 0x01;		// LED 켜기
		else PORTB = 0x00;			// LED 끄기
	}
}

int main(void)
{
	DDRB = 0x01; 				// PB0 핀을 출력으로 설정
	PORTB = 0x00;				// LED는 끈 상태에서 시작
	
	// 분주비를 1024로 설정
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);

	TIMSK |= (1 << TOIE0);			// 오버플로 인터럽트 허용
	
	sei();							// 전역적으로 인터럽트 허용
	
	while(1){ }
	return 0;
}