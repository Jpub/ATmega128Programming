#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void UART0_init(void);
void UART0_transmit(char data);

FILE OUTPUT \
= FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

#define set_bit(value, bit) ( _SFR_BYTE(value) |= _BV(bit) )
#define clear_bit(value, bit) ( _SFR_BYTE(value) &= ~_BV(bit) )
#define invert_bit(value, bit) ( _SFR_BYTE(value) ^= _BV(bit) )
#define read_bit(value, bit) ( (_SFR_BYTE(value) & _BV(bit)) >> bit )

#define BINARY_PATTERN "0b%d%d%d%d%d%d%d%d"
#define BYTE2BINARY(byte) \
(byte & 0x80 ? 1 : 0), \
(byte & 0x40 ? 1 : 0), \
(byte & 0x20 ? 1 : 0), \
(byte & 0x10 ? 1 : 0), \
(byte & 0x08 ? 1 : 0), \
(byte & 0x04 ? 1 : 0), \
(byte & 0x02 ? 1 : 0), \
(byte & 0x01 ? 1 : 0)

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
	
	unsigned char value = 0b00001111;
	printf("Original       : "BINARY_PATTERN"\n\r", BYTE2BINARY(value));
	
	set_bit(value, 6);			// 비트 세트
	printf("Set 7th bit    : "BINARY_PATTERN"\n\r", BYTE2BINARY(value));
	clear_bit(value, 2);			// 비트 클리어
	printf("Clear 3rd bit  : "BINARY_PATTERN"\n\r", BYTE2BINARY(value));
	invert_bit(value, 0);			// 비트 반전
	printf("Invert 1st bit : "BINARY_PATTERN"\n\r", BYTE2BINARY(value));
	int read = read_bit(value, 5);		// 비트 검사/읽기
	printf("6th bit is     : %d\n\r", read);
	
	while(1){}
	return 0;
}