#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	// PORT B의 3개 핀(PB0 ~ PB2)만을 출력으로 설정하고
	// 나머지 5개 핀은 디폴트값을 유지한다.
	DDRB |= 0x07;

	// PORT F의 3개 핀(PF0 ~ PF2)만을 입력으로 설정하고
	// 나머지 5개 핀은 디폴트값을 유지한다.
	DDRF &= ~0x07;

	// PORT F의 PF0 핀에 연결된 내부 풀업저항을 사용하도록 설정한다.
	PORTF |= 0x01;

	while (1)
	{
		// PORT B에 연결된 8개 핀 중 PB0 ~ PB2 까지의 3개 LED에만
		// 버튼의 상태를 반영하여 출력하고
		// 나머지 5개 핀의 출력은 이전 상태로 유지한다.
		PORTB = (PORTB & 0xF8) + (PINF & 0x07);
	}

	return 0;
}