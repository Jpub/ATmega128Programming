#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	// 포트 B를 출력으로 설정
	// DDRB를 DDRE 또는 DDRF로 변경하여 다른 포트를 테스트 할 수 있다.
	DDRB = 0xFF;

	while(1)
	{
		// 8개 LED 끄기
		// PORTB를 PORTE 또는 PORTF로 변경하여 다른 포트를 테스트 할 수 있다.
		PORTB = 0x00;
		_delay_ms(1000);			// 1초 대기
		// 8개 LED 켜기
		// PORTB를 PORTE 또는 PORTF로 변경하여 다른 포트를 테스트 할 수 있다.
		PORTB = 0xFF;
		_delay_ms(1000);			// 1초 대기
	}
	
	return 0;
}