#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRC |= 0x83;				// 모터 제어 핀을 출력으로 설정
	PORTC |= 0x80;			// 모터 제어 활성화 (Enable)
	
	while(1)
	{
		PORTC |= 0x02;			// 시계방향
		for(int i = 0; i < 400; i++){		// 400 스텝 진행
			PORTC |= 0x01;			// 상승 에지
			_delay_us(2500);
			PORTC &= ~0x01;			// 하강 에지
			_delay_us(2500);
		}
		_delay_ms(1000);

		PORTC &= ~0x02;			// 반시계방향
		for(int i = 0; i < 400; i++){		// 400 스텝 진행
			PORTC |= 0x01;			// 상승 에지
			_delay_us(2500);
			PORTC &= ~0x01;			// 하강 에지
			_delay_us(2500);
		}
		_delay_ms(1000);
	}
	
	return 0;
}