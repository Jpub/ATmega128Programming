#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	// 파형 출력 핀인 PB5(OC1A 핀), PB6 핀(OC1B 핀)을 출력으로 설정
	DDRB |= (1 << PB5) | (1 << PB6);

	// 타이머/카운터 1번을 8비트 위상 교정 PWM 모드로 설정
	TCCR1A |= (1 << WGM10);
	
	// 비교일치 A는 비반전 모드, 비교일치 B는 반전 모드로 설정
	TCCR1A |= (1 << COM1A1);
	TCCR1A |= (1 << COM1B1) | (1 << COM1B0);
	
	// 분주비를 256으로 설정
	TCCR1B |= (1 << CS12); // | (1 << CS10);

	int dim = 0;				// 현재 LED 밝기
	int direction = 1;			// 밝기 증가(1) 또는 감소(-1)
	
	while(1)
	{
		OCR1A = dim;			// 듀티 사이클 설정
		OCR1B = dim;
		
		_delay_ms(10);
		
		dim += direction;			// 밝기 변화
		
		if(dim == 0) direction = 1;
		if(dim == 255) direction = -1;
	}

	return 0;
}