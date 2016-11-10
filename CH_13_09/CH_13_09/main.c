#include <avr/io.h>
#include <avr/interrupt.h>

#define MILLIS_INCREMENT_PER_OVERFLOW	1
#define MICROS_INCREMENT_PER_OVERFLOW	24

// 프로그램 시작 이후의 경과 시간
volatile unsigned long timer0_millis = 0;
volatile int timer0_micros = 0;

ISR(TIMER0_OVF_vect)
{
	unsigned long m = timer0_millis;
	int f = timer0_micros;
	
	m += MILLIS_INCREMENT_PER_OVERFLOW;	// 밀리초 단위 시간 증가
	f += MICROS_INCREMENT_PER_OVERFLOW;	// 마이크로초 단위 시간 증가
	
	// 마이크로초가 1000을 넘어가면 밀리초를 증가시킴
	m += (f / 1000);
	f = f % 1000;
	
	timer0_millis = m;
	timer0_micros = f;
}

unsigned long millis()
{
	unsigned long m;
	uint8_t oldSREG = SREG;		// 상태 레지스터 값 저장
	
	// timer0_millis 값을 읽는 동안
	// timer0_millis 값이 변하지 않도록 인터럽트를 비활성화
	cli();
	
	m = timer0_millis;
	
	SREG = oldSREG;			// 이전 상태 레지스터 값 복원
	
	return m;					// 프로그램 시작 후 경과 시간
}

void init_timer0()
{
	TCCR0 |= (1 << CS02);			// 분주비를 64로 설정
	TIMSK |= (1 << TOIE0);			// 오버플로 인터럽트 허용

	sei();							// 전역적으로 인터럽트 허용
}

int main(void)
{
	uint8_t state = 0;			// LED 상태
	
	init_timer0();				// 타이머/카운터 0번 초기화

	DDRB = 0x01; 				// PB0 핀을 출력으로 설정
	PORTB = 0x00;				// LED는 끈 상태에서 시작
	
	unsigned long time_previous, time_current;
	time_previous = millis();		// 시작 시간
	
	while(1)
	{
		time_current = millis();		// 현재 시간
		
		if((time_current - time_previous) > 1000){	// 1초 경과
			time_previous = time_current;
			
			state = (state + 1) % 2;		// LED 상태 반전
			PORTB = state;
		}
	}
	
	return 1;
}