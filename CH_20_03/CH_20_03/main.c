#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define REGISTER_DDR	DDRD
#define REGISTER_PORT	PORTD

// ‘bit’ 위치의 비트를 1 또는 0으로 설정하기 위한 매크로 함수
#define set_bit(bit) 	( REGISTER_PORT |= _BV(bit) )
#define clear_bit(bit) 	( REGISTER_PORT &= ~_BV(bit) )

// ATmega128의 포트 D에 연결된 위치
#define SHIFT_CLOCK		2
#define LATCH_CLOCK		1
#define DATA			0

#define COL_ON			0
#define COL_OFF		1
#define ROW_ON			1
#define ROW_OFF		0

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

void init_port()
{
	// 제어 및 데이터 핀을 출력으로 설정
	REGISTER_DDR |= _BV(SHIFT_CLOCK) | _BV(LATCH_CLOCK) | _BV(DATA);
}

int main(void)
{
	init_port();
	
	uint8_t smile[] = {			// 스마일 문자 정의
		0b00111100,
		0b01000010,
		0b10010101,
		0b10100001,
		0b10100001,
		0b10010101,
		0b01000010,
		0b00111100 };
	
	while(1)
	{
		for(int i = 0; i < 8; i++){
			uint8_t col_data = ~(1 << i);
			
			ByteDataWrite(col_data);
			ByteDataWrite(smile[i]);
			
			_delay_ms(2);			// LED가 완전히 켜지기 위한 시간
		}
	}
}