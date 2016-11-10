#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// 모터 제어 허용/금지
#define EnableMotor1()			PORTE &= ~0x04
#define DisableMotor1()			PORTE |= 0x04
// 모터 회전 방향 설정
#define ForwardMotor1()			PORTE &= ~0x01
#define BackwardMotor1()		PORTE |= 0x01
// 모터 속도 설정
#define SpeedMotor1(s)			OCR3B = s

void InitializeTimer3(void)
{
	// 8비트 고속 PWM 모드
	TCCR3A |= (1 << WGM30);
	TCCR3B |= (1 << WGM32);
	
	// 비반전 모드
	// TOP : 0xFF, 비교일치값 : OCR3B 레지스터
	TCCR3A |= (1 << COM3B1);
	
	// 분주비 64
	TCCR3B |= (1 << CS31) | (1 << CS30);
	
	OCR3B = 0;
}

int main(void)
{
	// 모터 제어를 위한 핀들을 출력으로 설정
	DDRE |= (1 << PE0) | (1 << PE2) | (1 << PE4);

	InitializeTimer3();			// 3번 타이머/카운터 설정
	
	while(1)
	{
		EnableMotor1();
		ForwardMotor1();			// 시계방향 회전
		
		for(int i = 0; i < 256; i++){		// 속도 조절을 위한 PWM 신호 출력
			SpeedMotor1(i);			// 속도 제어 0~255
			_delay_ms(25);
		}
		
		_delay_ms(1000);
		
		SpeedMotor1(0);			// 모터 정지
		DisableMotor1();
		
		EnableMotor1();
		BackwardMotor1();			// 반시계방향 회전
		
		for(int i = 0; i < 256; i++){		// 속도 조절을 위한 PWM 신호 출력
			SpeedMotor1(i);			// 속도 제어 0~255
			_delay_ms(25);
		}
		
		_delay_ms(1000);
		
		SpeedMotor1(0);			// 모터 정지
		DisableMotor1();
	}

	return 0;
}