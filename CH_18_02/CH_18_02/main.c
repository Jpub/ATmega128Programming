#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>

volatile int interrupt_count = 0;		// 인터럽트 발생 횟수

ISR(TIMER0_OVF_vect)			// 타이머/카운터 0번 오버플로 인터럽트
{
	interrupt_count++;
}

int main(void)
{
	uint8_t numbers[]
	= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};
	int count = 0;
	DDRB = 0xFF;
	PORTB = numbers[0];			// 0에서 시작
	
	// 타이머/카운터 0번 인터럽트 설정
	// 분주비를 1024로 설정
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);
	TIMSK |= (1 << TOIE0);			// 오버플로 인터럽트 허용
	sei();							// 전역적으로 인터럽트 허용
	
	while(1)
	{
		if(interrupt_count >= 64){		// 1초 경과
			interrupt_count = 0;		// 인터럽트 발생 횟수 초기화
			
			count = (count + 1) % 10;		// 표시할 숫자
			PORTB = numbers[count];		// 숫자 데이터 출력
		}
	}
	
	return 0;
}