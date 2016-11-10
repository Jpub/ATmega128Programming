#define F_CPU 16000000
#define COL_OUT	PORTB		// 열 출력
#define ROW_IN		PINB		// 행 입력

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART1.h"

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

uint8_t keystate[4][4];			// 키 상태 저장

void read_key(void)
{
	for(int x = 0; x < 4; x++){
		// 해당 열에만 LOW를 출력하고 나머지는 HIGH 출력
		COL_OUT |= 0x0F;
		COL_OUT &= ~(0x01 << x);
		
		_delay_ms(10);
		
		uint8_t read = ROW_IN >> 4;		// 키 상태가 상위 4비트로 반환
		for(int y = 0; y < 4; y++){
			if(bit_is_set(read, y)){
				keystate[x][y] = 0;		// 버튼이 눌러지지 않으면 HIGH
			}
			else{
				keystate[x][y] = 1;		// 버튼이 눌러지면 LOW
			}
		}
	}
}

void print_key(void)			// 버튼 상태 출력
{
	for(int x = 0; x < 4; x++){
		for(int y = 0; y < 4; y++){
			printf("%c ", (keystate[y][x] ? 'O' : '.'));
		}
		printf("\r\n");
	}
	printf("\r\n\r\n");
}

int main(void)
{
	// 열에 해당하는 핀은 출력으로 행에 해당하는 핀은 입력으로 설정
	DDRB = 0x0F;
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init();				// UART 통신 초기화
	
	while(1)
	{
		read_key();
		print_key();
		
		_delay_ms(1000);
	}

	return 0;
}