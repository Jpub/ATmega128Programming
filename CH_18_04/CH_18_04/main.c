#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define SEGMENT_DELAY 5		// LED가 완전히 켜지기 위한 시간
uint8_t numbers[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};

void display_digit(int position, int number)
{
	// position : 출력할 자리 (1 ~ 4)
	// number : 출력할 숫자 (0 ~ 9)
	PORTC |= 0x0F;
	PORTC &= ~(1 << (position - 1));	// 자리 선택
	
	PORTD = numbers[number];		// 숫자 출력
}

int main(void)
{
	DDRD = 0xFF;				// 세그먼트 제어 핀 8개를 출력으로 설정
	DDRC = 0x0F;				// 자릿수 선택 핀 4개를 출력으로 설정
	
	while(1)
	{
		display_digit(1, 1);			// 첫 번째 자리에 ‘1’ 출력
		_delay_ms(SEGMENT_DELAY);
		display_digit(2, 2);			// 두 번째 자리에 ‘2’ 출력
		_delay_ms(SEGMENT_DELAY);
		display_digit(3, 3);			// 세 번째 자리에 ‘3’ 출력
		_delay_ms(SEGMENT_DELAY);
		display_digit(4, 4);			// 네 번째 자리에 ‘4’ 출력
		_delay_ms(SEGMENT_DELAY);
	}
	
	return 0;
}