#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	int dim = 0; 				// 현재 LED 밝기
	int direction = 1;			// 밝기 증가(1) 또는 감소(-1)

	// 파형 출력 핀인 PB4 핀(OC0 핀)을 출력으로 설정
	DDRB |= (1 << PB4);

	// 타이머/카운터 0번을 고속 PWM 모드로 설정
	TCCR0 |= (1 << WGM01) | (1 << WGM00);
	// 비반전 모드
	TCCR0 |= (1 << COM01);
	// 분주비를 1024로 설정
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);

	while(1)
	{
		OCR0 = dim;				// 듀티 사이클 설정
		_delay_ms(10);
		
		dim += direction;			// 밝기 변화
		
		if(dim == 0) direction = 1;
		if(dim == 255) direction = -1;
	}

	return 0;
}