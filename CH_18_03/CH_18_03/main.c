#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	uint8_t numbers[]
	= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};
	int i, j;
	
	DDRD = 0xFF;				// 세그먼트 제어 핀 8개를 출력으로 설정
	DDRC = 0x0F;				// 자릿수 선택 핀 4개를 출력으로 설정
	
	while(1)
	{
		for(i = 0; i < 4; i++){
			// 해당 자리에만 0(GND)을 출력하고 나머지에는 1을 출력하여 자리 선택
			PORTC |= 0x0F;
			PORTC &= ~(1 << i);

			for(j = 0; j < 10; j++){
				PORTD = numbers[j];		// 숫자 데이터 출력
				_delay_ms(100);			// 0.1초 간격으로 전환
			}
		}
	}
	
	return 0;
}