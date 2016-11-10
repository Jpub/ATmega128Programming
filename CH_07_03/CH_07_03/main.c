#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	char pattern_value = 0x01;

	DDRB = 0xFF;			// PORT B 핀을 출력으로 설정

	while (1)
	{
		PORTB = pattern_value;	// 현재 패턴 표시

		// 다음 패턴 값 생성
		pattern_value <<= 1;
		if(pattern_value == 0) pattern_value = 0x01;

		_delay_ms(500);
	}

	return 0;
}