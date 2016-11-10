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
	
	while(1)
	{
		for(int col = 0; col < 8; col++){
			for(int row = 0; row < 8; row++){
				// 공통 양극 방식
				// column에는 0을, ROW에는 1을 출력해야 해당 LED가 켜짐
				write_column_data(~(1 << col));
				write_row_data(1 << row);
				
				_delay_ms(100);
			}
		}
	}
}