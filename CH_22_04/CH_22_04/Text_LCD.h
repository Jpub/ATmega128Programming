#ifndef TEXT_LCD_H_
#define TEXT_LCD_H_

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <util/delay.h>

#define PORT_DATA		PORTD		// 데이터 핀 연결 포트
#define PORT_CONTROL		PORTC		// 제어 핀 연결 포트
#define DDR_DATA		DDRD		// 데이터 핀의 데이터 방향
#define DDR_CONTROL		DDRC		// 제어 핀의 데이터 방향

#define RS_PIN			0		// RS 제어 핀의 비트 번호
#define RW_PIN			1		// R/W 제어 핀의 비트 번호
#define E_PIN			2		// E 제어 핀의 비트 번호

#define COMMAND_CLEAR_DISPLAY	0x01
#define COMMAND_8_BIT_MODE		0x38	// 8비트, 2라인, 5x8 폰트
#define COMMAND_4_BIT_MODE		0x28	// 4비트, 2라인, 5x8 폰트

#define COMMAND_DISPLAY_ON_OFF_BIT		2
#define COMMAND_CURSOR_ON_OFF_BIT		1
#define COMMAND_BLINK_ON_OFF_BIT		0

extern uint8_t MODE;

void LCD_pulse_enable(void);			// E를 HIGH -> LOW로 변화
void LCD_write_data(uint8_t data);		// 데이터 전송으로 문자 출력
void LCD_write_command(uint8_t command);	// 명령어 실행
void LCD_clear(void);				// 화면을 공백 문자로 지움
void LCD_init(void);				// 텍스트 LCD 초기화
void LCD_write_string(char *string);		// 문자열 출력
void LCD_goto_XY(uint8_t row, uint8_t col);	// 임의의 위치로 커서 이동

#endif /* TEXT_LCD_H_ */