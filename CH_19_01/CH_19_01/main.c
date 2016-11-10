#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

// ‘bit’ 위치의 비트를 1 또는 0으로 설정하기 위한 매크로 함수
#define set_bit(bit) 	( PORTB |= _BV(bit) )
#define clear_bit(bit) 	( PORTB &= ~_BV(bit) )

// ATmega128의 포트 B에 연결된 위치
#define SHIFT_CLOCK		0
#define LATCH_CLOCK		1
#define DATA			2

void ShiftClock(void)
{
	set_bit(SHIFT_CLOCK);			// 이동 클록을 HIGH로
	clear_bit(SHIFT_CLOCK);		// 이동 클록을 LOW로
}

void LatchClock(void)
{
	set_bit(LATCH_CLOCK);			// 래치 클록을 HIGH로
	clear_bit(LATCH_CLOCK);		// 래치 클록을 LOW로
}

void ByteDataWrite(uint8_t data)		// 1 바이트 데이터 출력
{
	for(uint8_t i = 0; i < 8; i++){
		if(data & 0b10000000) 		// MSB부터 1비트 출력
		set_bit(DATA);
		else
		clear_bit(DATA);
		
		ShiftClock();				// 1비트 출력 후 비트 이동
		data = data << 1;			// 다음 출력할 비트를 MSB로 이동
	}
	
	LatchClock();				// 1바이트 전달 후 실제 출력 발생
}

int main(void)
{
	// 제어 및 데이터 핀을 출력으로 설정
	DDRB |= _BV(SHIFT_CLOCK) | _BV(LATCH_CLOCK) | _BV(DATA);

	uint8_t index = 0;			// 켜질 LED의 위치
	while (1)
	{
		uint8_t pattern = 1 << index;		// 출력 패턴 결정
		index = (index + 1) % 8;		// 출력 패턴에서의 위치 결정
		
		ByteDataWrite(pattern);		// 바이트 데이터 출력
		
		_delay_ms(1000);
	}
	
	return 0;
}