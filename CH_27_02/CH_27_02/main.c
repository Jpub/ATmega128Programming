#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "UART0.h"
#include "UART1.h"

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

void process_data(char *buffer)
{
	if(strlen(buffer) < 6) return;

	// "$GPGGA"로 시작하는 경우에만 터미널로 출력
	if(strncmp(buffer, "$GPGGA", 6) == 0){
		printf("%s\r\n", buffer);
	}
}

int main(void)
{
	uint8_t data, position = 0;
	char buffer[200];
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART0_init();				// UART0 통신 초기화
	UART1_init();				// UART1 통신 초기화
	
	while (1)
	{
		data = UART0_receive();
		
		if(data != '\r'){
			if(data == '\n'){			// 문장의 끝
				buffer[position] = '\0';		// 문자열의 끝 표시
				process_data(buffer);		// GGA 문장 찾기
				position = 0;			// 버퍼 초기화
			}
			else{
				buffer[position++] = data;		// 데이터를 버퍼에 저장
			}
		}
	}

	return 0;
}