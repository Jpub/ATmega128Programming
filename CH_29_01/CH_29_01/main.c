#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "UART1.h"

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

volatile int bitCount;			// 수신 데이터 비트 수
volatile unsigned long receivedData;	// 수신 데이터 (4바이트 크기)

void print_received_data(int repeat)
{
	if(repeat == 1)				// 반복 데이터
	printf("** Repeat...\r\n");
	else								// 일반 데이터
	printf("0x%lX\r\n", receivedData);
}

ISR(INT0_vect)
{
	int time = TCNT0;			// 인터럽트 발생 시간
	int overflow = TIFR & (1 << TOV0);	// 오버플로 발생 여부 검사
	
	if(bitCount == 32){			// 리드 코드 검사
		// 일반 데이터 리드 코드인 경우
		// 일반 데이터 리드 코드 길이 13.5ms는 약 211 클록에 해당함
		if ((time > 201) && (time < 221) && (overflow == 0)) {
			receivedData = 0;
			bitCount = 0;
		}
		// 반복 데이터 리드 코드인 경우
		// 반복 데이터 리드 코드 길이 11.25ms는 약 176 클록에 해당함
		else if((time > 166) && (time < 186) && (overflow == 0)){
			print_received_data(1);
		}
		else{
			bitCount = 32;			// 잘못된 신호인 경우 재시작
		}
	}
	else{							// 논리 데이터 검사
		// 논리 0 : 약 18 클록, 논리 1 : 약 35 클록
		if((time > 40) || (overflow != 0))	// 신호 간격이 너무 긴 경우 재시작
		bitCount = 32;
		else{
			if(time > 26)			// 1 수신 ( ≈ (18 + 35) / 2 )
			receivedData = (receivedData << 1) + 1;
			else						// 0 수신
			receivedData = (receivedData << 1);
			
			if(bitCount == 31)			// 4 바이트 데이터 수신 완료
			print_received_data(0);

			bitCount++;
		}
	}
	
	TCNT0 = 0;                  		// 0번 타이머/카운터 클리어
	// 0번 타이머/카운터 오버플로 플래그 클리어
	// ISR 루틴이 호출되지 않으므로 자동으로 클리어 되지 않는다.
	TIFR = TIFR | (1 << TOV0);
}

int main(void)
{
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init();				// UART 통신 초기화
	
	// 0번 타이머/카운터 분주비를 1024로 설정
	TCCR0 |= (1 << CS00) | (1 << CS01) | (1 << CS02);
	
	// PD0 핀에 연결된 적외선 수신기에 대한 외부 인터럽트 설정
	EIMSK |= (1 << INT0);			// INT0 인터럽트 활성화
	EICRA |= (1 << ISC01);			// 하강 에지에서 인터럽트 발생
	sei();							// 전역적으로 인터럽트 허용

	bitCount = 32;               		// 시작 신호 대기 상태
	
	printf("** Initialization Completed... \r\n");
	
	while(1);
	return 0;
}