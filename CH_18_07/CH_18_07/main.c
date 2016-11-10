#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SEGMENT_DELAY 5		// LED가 완전히 켜지기 위한 시간
#define MILLIS_PER_OVERFLOW		16
#define MICROS_PER_OVERFLOW 	384

uint8_t numbers[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};

volatile uint32_t millis = 0;
volatile uint16_t micros = 0;

ISR(TIMER0_OVF_vect)			// 타이머/카운터 0번 오버플로 인터럽트
{
	micros += MICROS_PER_OVERFLOW;
	millis += MILLIS_PER_OVERFLOW;
	
	millis += (micros / 1000);
	micros %= 1000;
}

void display_digit(int position, int number)
{
	PORTC |= 0x0F;
	PORTC &= ~(1 << (position - 1));
	
	PORTD = numbers[number];
}

int main(void)
{
	DDRD = 0xFF;				// 세그먼트 제어핀 8개를 출력으로 설정
	DDRC = 0x0F;				// 자릿수 선택핀 4개를 출력으로 설정
	
	// 타이머/카운터 0번 인터럽트 설정
	// 분주비를 1024로 설정
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);
	TIMSK |= (1 << TOIE0);			// 오버플로 인터럽트 허용
	sei();							// 전역적으로 인터럽트 허용
	
	while(1)
	{
		int temp = millis / 1000;

		int minutes = (temp / 60) % 60;	// 분 계산
		int seconds = temp % 60;		// 초 계산
		
		// 분을 두 자리로 나누어 표시
		display_digit(1, minutes / 10);
		_delay_ms(SEGMENT_DELAY);
		display_digit(2, minutes % 10);
		_delay_ms(SEGMENT_DELAY);

		// 초를 두 자리로 나누어 표시
		display_digit(3, seconds / 10);
		_delay_ms(SEGMENT_DELAY);
		display_digit(4, seconds % 10);
		_delay_ms(SEGMENT_DELAY);
	}
	
	return 0;
}