#define F_CPU 16000000L
#include <avr/io.h>

int main(void)
{
	DDRB |= 0x01;				// LED 연결 핀을 출력으로 설정
	
	while(1)
	{
		uint8_t mask = (1 << ACO);
		if((ACSR & mask) == mask){		// AIN0 > AIN1
			PORTB |= 0x01;			// LED 켜기
		}
		else{
			PORTB &= ~0x01;			// AIN0 <= AIN1
		}
	}

	return 0;
}