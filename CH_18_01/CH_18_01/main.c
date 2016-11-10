#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	uint8_t numbers[]
	= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};
	int count = 0;				// 현재 표시할 숫자
	DDRB = 0xFF;				// 제어핀 8개를 출력으로 설정
	
	while(1)
	{
		PORTB = numbers[count];		// 숫자 데이터 출력
		count = (count + 1) % 10;		// 다음에 표시할 숫자
		
		_delay_ms(1000);
	}
	
	return 0;
}