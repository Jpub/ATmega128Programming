#define F_CPU 16000000L
#define TERMINATOR '\r'

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "UART1.h"

int main(void)
{
	uint8_t counter = 100; 		// 카운터
	int index = 0;				// 수신 버퍼에 저장할 위치
	int process_data = 0;			// 문자열 처리
	char buffer[20] = "";			// 수신 데이터 버퍼
	char data;					// 수신 데이터
	
	UART1_init();				// UART 통신 초기화
	
	UART1_print_string("Current Counter Value : ");
	UART1_print_1_byte_number(counter);
	UART1_print_string("\r\n");
	
	while(1)
	{
		data = UART1_receive();		// 데이터 수신
		if(data == TERMINATOR){		// 종료 문자를 수신한 경우
			buffer[index] = '\0';
			process_data = 1;			// 수신 문자열 처리 지시
		}
		else{
			buffer[index] = data;		// 수신 버퍼에 저장
			index++;
		}
		
		if(process_data == 1){		// 문자열 처리
			if(strcmp(buffer, "DOWN") == 0){	// 카운터 감소
				counter--;
				UART1_print_string("Current Counter Value : ");
				UART1_print_1_byte_number(counter);
				UART1_print_string("\r\n");
			}
			else if(strcmp(buffer, "UP") == 0){	// 카운터 증가
				counter++;
				UART1_print_string("Current Counter Value : ");
				UART1_print_1_byte_number(counter);
				UART1_print_string("\r\n");
			}
			else{					// 잘못된 명령어
				UART1_print_string("** Unknown Command **");
				UART1_print_string("\r\n");
			}
			index = 0;
			process_data = 0;
		}
	}

	return 0;
}