#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define C4		262 // 261.63 Hz
#define D4		294 // 293.66 Hz
#define E4			330 // 329.63 Hz
#define F4			349 // 349.23 Hz
#define G4		392 // 392.00 Hz
#define A4		440 // 440.00 Hz
#define B4			494 // 493.88 Hz
#define C5		523 // 523.25 Hz
#define PAUSE 	0
#define PRESCALER 	8

const int melody[] = {C4, D4, E4, F4, G4, A4, B4, C5, PAUSE};

void init_music()
{
	DDRB |= (1 << PB6); 			// PBB 핀(OC1B 핀)을 출력으로 설정
	
	// COM1Bn = 01 : 비교일치 발생 시 출력 반전
	TCCR1A |= _BV(COM1B0);
	
	// WGM1n = 0100 : 4번 CTC 모드
	// CS1n = 010 : 분주비 8
	TCCR1B |= _BV(WGM12) | _BV(CS11);
}

void play_music(const int* pMusicNotes)
{
	int note;

	while(*pMusicNotes){
		note = *pMusicNotes;
		pMusicNotes++;

		if(note == PAUSE){
			OCR1A = 0;			// 재생 중지, 듀티 사이클 0%
		}
		else{
			// 재생 주파수 계산
			int ocr_value = F_CPU / 2 / PRESCALER / note;

			// 재생 주파수 설정
			OCR1A = ocr_value;
		}

		_delay_ms(500);			// 0.5초 간격으로 단음 재생
	}

	OCR1A = 0;
}

int main()
{
	init_music();				// 포트 및 타이머 설정
	
	while(1)
	{
		play_music(melody);
		_delay_ms(1000);
	}
	
	return 0;
}