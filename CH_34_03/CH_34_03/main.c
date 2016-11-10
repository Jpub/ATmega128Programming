#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	uint8_t state = 0x00;			// LED 상태
	DDRD = 0xFF;				// PORT D 8개 핀을 출력으로 설정

	while(1)
	{
		state = (state ^ 0xFF);		// 8개 핀의 출력값 반전

		PORTD = state;			// LED 점멸

		_delay_ms(1000);			// 1초 대기
	}

	return 0;
}