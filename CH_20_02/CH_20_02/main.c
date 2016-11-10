#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define DDR_ROW		DDRC
#define DDR_COL		DDRD
#define PORT_ROW		PORTC
#define PORT_COL		PORTD

#define COL_ON			0
#define COL_OFF		1
#define ROW_ON			1
#define ROW_OFF		0

void write_column_data(uint8_t data)
{
	PORT_COL = data;
}

void write_row_data(uint8_t data)
{
	PORT_ROW = data;
}

void init_port(void)
{
	// ROW 관련 포트 설정
	DDR_ROW = 0xFF;			// 출력으로 설정
	write_row_data(0xFF * ROW_OFF);	// OFF 값 출력

	// COLUMN 관련 포트 설정
	DDR_COL = 0xFF;			// 출력으로 설정
	write_column_data(0xFF * COL_OFF);	// OFF 값 출력
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
			
			write_column_data(col_data);
			write_row_data(smile[i]);
			
			_delay_ms(2);			// LED가 완전히 켜지기 위한 시간
		}
	}
}