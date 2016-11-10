#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define ROTATION_DELAY 	1000	// 1초 대기
#define PULSE_MIN		1300	// 최소 펄스 길이
#define PULSE_MID		3000
#define PULSE_MAX		4700	// 최대 펄스 길이

void InitializeTimer1(void)
{
	// 모드 14, 고속 PWM 모드
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	
	// 비반전 모드
	// TOP : ICR1, 비교일치값 : OCR1A 레지스터
	TCCR1A |= (1 << COM1A1);
	
	TCCR1B |= (1 << CS11);		// 분주율 8, 2MHz
	
	ICR1 = 39999;				// 20ms 주기
}

int main(void)
{
	// 모터 제어 핀을 출력으로 설정
	DDRB |= (1 << PB5);

	InitializeTimer1();			// 1번 타이머/카운터 설정
	
	while(1)
	{
		OCR1A = PULSE_MIN;		// 0도
		_delay_ms(ROTATION_DELAY);
		
		OCR1A = PULSE_MID;		// 90도
		_delay_ms(ROTATION_DELAY);
		
		OCR1A = PULSE_MAX;		// 180도
		_delay_ms(ROTATION_DELAY);
	}

	return 0;
}