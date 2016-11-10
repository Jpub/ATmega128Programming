#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "I2C_RTC.h"
#include "UART1.h"

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

#define MCP23017_ADDRESS		0x20
#define MCP23017_IODIRA		0x00
#define MCP23017_IODIRB		0x01
#define MCP23017_GPPUA		0x0C
#define MCP23017_GPPUB		0x0D
#define MCP23017_GPIOA			0x12
#define MCP23017_GPIOB			0x13

int main(void)
{
	I2C_init();					// I2C 통신
	UART1_init();				// UART 통신 초기화
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	I2C_start();				// I2C 시작
	I2C_transmit(MCP23017_ADDRESS << 1);	// I2C 주소 전송. 쓰기 모드
	I2C_transmit(MCP23017_IODIRA);	// IODIRA 레지스터 주소
	I2C_transmit(0x00); 			// 뱅크 A 핀을 출력으로 설정
	I2C_transmit(0xFF);			// 뱅크 B 핀을 입력으로 설정
	I2C_stop();					// I2C 정지
	
	I2C_start();
	I2C_transmit(MCP23017_ADDRESS << 1);	// I2C 주소 전송. 쓰기 모드
	I2C_transmit(MCP23017_GPPUB); 	// GPPUB 레지스터 주소
	I2C_transmit(0); 			// 뱅크 B 핀의 풀업 저항 해제
	I2C_stop();
	
	while(1)
	{
		// 뱅크 B의 버튼 상태 읽기
		I2C_start();
		I2C_transmit(MCP23017_ADDRESS << 1);	// I2C 주소 전송. 쓰기 모드
		I2C_transmit(MCP23017_GPIOB);		// GPIOB 레지스터 주소
		I2C_stop();
		
		I2C_start();
		I2C_transmit((MCP23017_ADDRESS << 1) + 1);	// I2C 주소 전송. 읽기 모드
		uint8_t button_state = I2C_receive_NACK();	// 버튼 상태 읽기
		I2C_stop();
		
		printf("%d\r\n", button_state);		// 버튼 상태 출력
		
		// 뱅크 A의 LED에 뱅크 B의 버튼 상태 출력
		I2C_start();
		I2C_transmit(MCP23017_ADDRESS << 1);	// I2C 주소 전송. 쓰기 모드
		I2C_transmit(MCP23017_GPIOA);		// GPIOA 레지스터 주소
		I2C_transmit(button_state);			// 버튼 상태를 LED로 출력
		I2C_stop();
		
		_delay_ms(500);
	}
	
	return 0;
}