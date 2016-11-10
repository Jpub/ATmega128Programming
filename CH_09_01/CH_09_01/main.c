#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

void UART1_init(void);
void UART1_transmit(char data);
unsigned char UART1_receive(void);

void UART1_init(void)
{
	UBRR1H = 0x00;			// 9600 보율로 설정
	UBRR1L = 207;
	
	UCSR1A |= _BV(U2X1);			// 2배속 모드
	// 비동기, 8비트 데이터, 패리티 없음, 1비트 정지 비트 모드
	UCSR1C |= 0x06;
	
	UCSR1B |= _BV(RXEN1);		// 송수신 가능
	UCSR1B |= _BV(TXEN1);
}

void UART1_transmit(char data)
{
	while( !(UCSR1A & (1 << UDRE1)) );	// 송신 가능 대기
	UDR1 = data;				// 데이터 전송
}

unsigned char UART1_receive(void)
{
	while( !(UCSR1A & (1<<RXC1)) );	// 데이터 수신 대기
	return UDR1;
}

int main(void)
{
	UART1_init();				// UART1 초기화
	
	while(1)
	{
		UART1_transmit(UART1_receive());
	}
	
	return 0;
}