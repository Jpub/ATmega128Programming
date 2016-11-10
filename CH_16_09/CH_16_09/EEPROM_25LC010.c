#include "EEPROM_25LC010.h"

void SPI_Init(void)
{
	DDRB |= (1 << SPI_SS);			// SS 핀을 출력으로 설정
	// SS핀은 HIGH로 설정하여 EEPROM이 선택되지 않은 상태로 시작
	PORTB |= (1 << SPI_SS);

	DDRB |= (1 << SPI_MOSI);		// MOSI 핀을 출력으로 설정
	DDRB &= ~(1 << SPI_MISO);		// MISO 핀을 입력으로 설정
	DDRB |= (1 << SPI_SCK);		// SCK 핀을 출력으로 설정

	SPCR |= (1 << MSTR);			// 마스터 모드
	SPCR |= (1 << SPE);			// SPI 활성화
}

void EEPROM_changeByte(uint8_t byte)
{
	SPDR = byte;				// 데이터 전송 시작
	loop_until_bit_is_set(SPSR, SPIF);	// 전송 완료 대기
}

void EEPROM_sendAddress(uint8_t address)
{
	EEPROM_changeByte(address);
}

uint8_t EEPROM_readByte(uint8_t address)
{
	EEPROM_Select();			// EEPROM 선택
	EEPROM_changeByte(EEPROM_READ);	// 읽기 명령어 전송
	EEPROM_sendAddress(address);		// 메모리 주소 전송
	// 마스터에서 바이트 값을 전송하여야 슬레이브로부터 바이트 값을 받을 수 있다.
	// 전송하는 값은 의미가 없으므로 0을 전송한다.
	EEPROM_changeByte(0);
	EEPROM_DeSelect();			// EEPROM 선택 해제

	return SPDR;
}

void EEPROM_writeEnable(void)
{
	EEPROM_Select();			// Slave Select를 LOW로
	EEPROM_changeByte(EEPROM_WREN);	// 쓰기 가능하도록 설정
	EEPROM_DeSelect();			// Slave Select를 HIGH로
}

void EEPROM_writeByte(uint8_t address, uint8_t data)
{
	EEPROM_writeEnable();			// 쓰기 가능 모드로 설정

	EEPROM_Select();			// EEPROM 선택
	EEPROM_changeByte(EEPROM_WRITE);	// 쓰기 명령 전송
	EEPROM_sendAddress(address);		// 주소 전송
	EEPROM_changeByte(data);		// 데이터 전송
	EEPROM_DeSelect();			// EEPROM 선택 해제

	// 쓰기가 완료될 때까지 대기
	while (EEPROM_readStatus() & _BV(EEPROM_WRITE_IN_PROGRESS));
}

uint8_t EEPROM_readStatus(void)
{
	EEPROM_Select();			// EEPROM 선택
	EEPROM_changeByte(EEPROM_RDSR);	// 상태 레지스터 읽기 명령 전송
	EEPROM_changeByte(0);		// 상태 레지스터 값 읽기
	EEPROM_DeSelect();			// EEPROM 선택 해제

	return SPDR;
}

void EEPROM_eraseAll(void)
{
	uint8_t i;
	uint16_t pageAddress = 0;

	while (pageAddress < EEPROM_TOTAL_BYTE){
		EEPROM_writeEnable();			// 쓰기 가능 모드로 설정
		EEPROM_Select();				// EEPROM 선택
		EEPROM_changeByte(EEPROM_WRITE);		// 쓰기 명령 전송
		EEPROM_sendAddress(pageAddress);		// 페이지 시작 주소 전송
		for (i = 0; i < EEPROM_PAGE_SIZE; i++)	// 페이지 단위 데이터 전송
		EEPROM_changeByte(0);
		EEPROM_DeSelect();				// EEPROM 선택 해제

		pageAddress += EEPROM_PAGE_SIZE;		// 페이지 변경
		// 쓰기 완료 대기
		while(EEPROM_readStatus() & _BV(EEPROM_WRITE_IN_PROGRESS));
	}
}