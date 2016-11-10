#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

char circular_shift_right(char pattern)
{
	char LSB = pattern & 0x01;		// LSB
	char MSB = LSB << 7;			// LSB 값을 MSB로 옮김
	
	char new_pattern = pattern >> 1;	// 패턴을 오른쪽으로 이동
	
	new_pattern |= MSB;			// MSB 추가

	return new_pattern;
}

int main(void)
{
	char pattern = 0xA0;

	DDRB = 0xFF;				// PORT B 핀을 출력으로 설정

	while (1)
	{
		PORTB = pattern;			// 현재 패턴 표시

		// 다음 인덱스 생성
		pattern = circular_shift_right(pattern);

		_delay_ms(500);
	}

	return 0;
}