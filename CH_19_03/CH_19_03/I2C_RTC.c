#include "I2C_RTC.h"

void I2C_init(void)
{
	DDRD |= (1 << I2C_SCL);	// SCL 핀을 출력으로 설정
	DDRD |= (1 << I2C_SDA);	// SDA 핀을 출력으로 설정

	TWBR = 32;			// I2C 클록 주파수 설정 200KHz
}

void I2C_start(void)
{
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);

	while( !(TWCR & (1 << TWINT)) );	// 시작 완료 대기
}

void I2C_transmit(uint8_t data)
{
	TWDR = data;
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);

	while( !(TWCR & (1 << TWINT)) );	// 전송 완료 대기
}

uint8_t I2C_receive_ACK(void)
{
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
	
	while( !(TWCR & (1 << TWINT)) );	// 수신 완료 대기
	
	return TWDR;
}

uint8_t I2C_receive_NACK(void)
{
	TWCR = _BV(TWINT) | _BV(TWEN);
	
	while( !(TWCR & (1 << TWINT)) );	// 수신 완료 대기
	
	return TWDR;
}

void I2C_stop(void)
{
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
}