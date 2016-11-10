#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

char circular_shift_left(char pattern)
{
	return ( (pattern << 1) | (pattern >> 7) );
}

int main(void)
{
	DDRB = 0xFF;				// PORT B 핀을 출력으로 설정
	DDRF &= ~0x04;			// PORT F의 PF2 핀을 입력으로 설정
	
	char pattern = 0x01;			// 초기 출력값
	PORTB = pattern;
	
	while (1)
	{
		if( (PINF & 0x04) >> 2 == 1){		// 버튼이 눌러진 경우
			// 새로운 패턴값을 생성하여 출력
			pattern = circular_shift_left(pattern);
			PORTB = pattern;
		}
	}

	return 0;
}