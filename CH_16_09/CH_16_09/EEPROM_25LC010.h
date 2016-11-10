#ifndef EEPROM_25LC010_H_
#define EEPROM_25LC010_H_

#include <avr/io.h>

// SPI를 위한 핀 정의
#define SPI_SS			PB0
#define SPI_MOSI		PB2
#define SPI_MISO		PB3
#define SPI_SCK			PB1

#define EEPROM_Select()		PORTB &= ~(1 << SPI_SS)	// LOW
#define EEPROM_DeSelect()	PORTB |= (1 << SPI_SS)		// HIGH

// 명령어
#define EEPROM_READ		0b00000011	// 읽기
#define EEPROM_WRITE		0b00000010	// 쓰기
#define EEPROM_WREN		0b00000110	// 쓰기 허용
#define EEPROM_RDSR		0b00000101	// 상태 레지스터 읽기

#define EEPROM_WRITE_IN_PROGRESS		0

#define EEPROM_PAGE_SIZE	16		// 페이지 크기 (byte)
#define EEPROM_TOTAL_BYTE	128 		// (byte)

void SPI_Init(void);				// SPI 초기화
void EEPROM_changeByte(uint8_t byte);		// 1바이트 데이터 교환
void EEPROM_sendAddress(uint8_t address);	// 8비트 주소 전송
uint8_t EEPROM_readStatus(void);			// EEPROM 상태 레지스터 읽기
void EEPROM_writeEnable(void);			// 쓰기 가능하도록 설정
uint8_t EEPROM_readByte(uint8_t address);	// 1바이트 데이터 읽기
void EEPROM_writeByte(uint8_t address, uint8_t byte);	// 1바이트 데이터 쓰기
void EEPROM_eraseAll(void); 			// EEPROM을 0으로 초기화

#endif