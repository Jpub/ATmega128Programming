#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	char patterns[] = {
		0xA0, 	0x50, 	0x28, 0x14,
		0x0A, 	0x05, 	0x82, 0x41
	};
	
	int index = 0;				// 현재 출력할 패턴

	DDRB = 0xFF;				// PORT B 핀을 출력으로 설정

	while (1)
	{
		PORTB = patterns[index];		// 현재 패턴 표시

		index = (index + 1) % 8;		// 다음 인덱스 생성

		_delay_ms(500);
	}

	return 0;
}