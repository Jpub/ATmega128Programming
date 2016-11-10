#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void UART0_init(void);
void UART0_transmit(char data);

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

void UART0_init(void)
{
	UBRR0H = 0x00; // 9,600 보율로 설정
	UBRR0L = 207;
	
	UCSR0A |= _BV(U2X0); // 2배속 모드
	// 비동기, 8비트 데이터, 패리티 없음, 1비트 정지 비트 모드
	UCSR0C |= 0x06;
	
	UCSR0B |= _BV(RXEN0); // 송수신 가능
	UCSR0B |= _BV(TXEN0);
}

void UART0_transmit(char data)
{
	while( !(UCSR0A & (1 << UDRE0)) ); // 송신 가능 대기
	UDR0 = data; // 데이터 전송
}

int main(void)
{
	UART0_init();				// UART0 초기화
	stdout = &OUTPUT;			// printf 사용 설정
	
	float r1, r2, r3, r4;
	
	r1 = 3 / 2;
	r2 = 3.0 / 2;
	r3 = 3 / 2.0;
	r4 = 3.0 / 2.0;
	
	printf("Result 1 : %f\n\r", r1);
	printf("Result 2 : %f\n\r", r2);
	printf("Result 3 : %f\n\r", r3);
	printf("Result 4 : %f\n\r", r4);
	
	while(1){}
	return 0;
}