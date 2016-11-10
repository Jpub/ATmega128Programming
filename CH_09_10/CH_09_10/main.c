#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "UART1.h"

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

int main(void)
{
	uint8_t counter = 100; 		// 카운터
	char buffer[20] = "";			// 수신 데이터 버퍼
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init();				// UART 통신 초기화
	
	printf("Current Counter Value : ");
	printf("%d\r\n", counter);
	
	while(1)
	{
		scanf("%s", buffer);			// 문자열 수신
		
		if(strcasecmp(buffer, "DOWN") == 0){	// 카운터 감소
			counter--;
			printf("Current Counter Value : ");
			printf("%d\r\n", counter);
		}
		else if(strcasecmp(buffer, "UP") == 0){	// 카운터 증가
			counter++;
			printf("Current Counter Value : ");
			printf("%d\r\n", counter);
		}
		else{						// 잘못된 명령어
			printf("** Unknown Command **\r\n");
		}
	}

	return 0;
}