#ifndef I2C_RTC_H_
#define I2C_RTC_H_

#define I2C_SCL			PD0
#define I2C_SDA			PD1

#include <avr/io.h>

void I2C_init(void);			// I2C 초기화
void I2C_start(void);			// I2C 시작
void I2C_transmit(uint8_t data);		// 1바이트 전송
uint8_t I2C_receive_NACK(void);		// 1바이트 수신 & NACK
uint8_t I2C_receive_ACK(void);		// 1바이트 수신 & ACK
void I2C_stop(void);			// I2C 정지

#endif