#include "Text_LCD.h"

void LCD_pulse_enable(void) 		// 하강 에지에서 동작
{
	PORT_CONTROL |= (1 << E_PIN);	// E를 HIGH로
	_delay_us(1);
	PORT_CONTROL &= ~(1 << E_PIN);	// E를 LOW로
	_delay_ms(1);
}

void LCD_write_data(uint8_t data)
{
	PORT_CONTROL |= (1 << RS_PIN);	// 문자 출력에서 RS는 1
	
	if(MODE == 8){
		PORT_DATA = data;			// 출력할 문자 데이터
		LCD_pulse_enable();
	}
	else{
		PORT_DATA = data & 0xF0;		// 상위 4비트
		LCD_pulse_enable();
		
		PORT_DATA = (data << 4) & 0xF0;	// 하위 4비트
		LCD_pulse_enable();
	}
	_delay_ms(2);
}

void LCD_write_command(uint8_t command)
{
	PORT_CONTROL &= ~(1 << RS_PIN);	// 명령어 실행에서 RS는 0
	
	if(MODE == 8){
		PORT_DATA = command;		// 데이터 핀에 명령어 전달
		LCD_pulse_enable();			// 명령어 실행
	}
	else{
		PORT_DATA = command & 0xF0;		// 상위 4비트
		LCD_pulse_enable();
		
		PORT_DATA = (command << 4) & 0xF0;	// 하위 4비트
		LCD_pulse_enable();
	}
	_delay_ms(2);
}

void LCD_clear(void)
{
	LCD_write_command(COMMAND_CLEAR_DISPLAY);
	_delay_ms(2);
}

void LCD_init(void)
{
	_delay_ms(50);				// 초기 구동 시간
	
	// 연결 핀을 출력으로 설정
	if(MODE == 8) DDR_DATA = 0xFF;
	else DDR_DATA |= 0xF0;
	PORT_DATA = 0x00;
	
	DDR_CONTROL |= (1 << RS_PIN) | (1 << RW_PIN) | (1 << E_PIN);
	
	// RW 핀으로 LOW를 출력하여 쓰기 전용으로 사용
	PORT_CONTROL &= ~(1 << RW_PIN);
	
	if(MODE == 8)
	LCD_write_command(COMMAND_8_BIT_MODE);		// 8비트 모드
	else{
		LCD_write_command(0x02);				// 4비트 모드 추가 명령
		LCD_write_command(COMMAND_4_BIT_MODE);		// 4비트 모드
	}
	
	// display on/off control
	// 화면 on, 커서 off, 커서 깜빡임 off
	uint8_t command = 0x08 | (1 << COMMAND_DISPLAY_ON_OFF_BIT);
	LCD_write_command(command);

	LCD_clear();			// 화면 지움

	// Entry Mode Set
	// 출력 후 커서를 오른쪽으로 옮김, 즉, DDRAM의 주소가 증가하며 화면 이동은 없음
	LCD_write_command(0x06);
}

void LCD_write_string(char *string)
{
	uint8_t i;
	for(i = 0; string[i]; i++)			// 종료 문자를 만날 때까지
	LCD_write_data(string[i]);		// 문자 단위 출력
}

void LCD_goto_XY(uint8_t row, uint8_t col)
{
	col %= 16;		// [0 15]
	row %= 2;		// [0 1]

	// 첫째 라인 시작 주소는 0x00, 둘째 라인 시작 주소는 0x40
	uint8_t address = (0x40 * row) + col;
	uint8_t command = 0x80 + address;
	
	LCD_write_command(command);	// 커서 이동
}