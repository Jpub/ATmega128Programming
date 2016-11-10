#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	char current_pattern_value;
	char next_pattern_value = 0x01;

	DDRB = 0xFF;				// PORT B 핀을 출력으로 설정

	while (1)
	{
		current_pattern_value = next_pattern_value;

		PORTB = current_pattern_value;	// 현재 패턴 표시

		// 다음 패턴값 생성
		next_pattern_value = current_pattern_value << 1;
		if(next_pattern_value == 0) next_pattern_value = 0x01;

		_delay_ms(500);
	}

	return 0;
}