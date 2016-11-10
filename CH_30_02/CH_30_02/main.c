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
#define PAUSE 0

#define PRESCALER 	8

const int melody[] = {C4, D4, E4, F4, G4, A4, B4, C5, PAUSE};

void init_music()
{
	DDRB |= (1 << PB6); 			// PB6 핀(OC1B 핀)을 출력으로 설정

	// 1번 타이머/카운터에서 비교일치가 발생한 경우 OC1A, OC1B, OC1C 핀의 출력이
	// 상향 카운트에서 세트 되고, 하향 카운트에서 클리어 된다.
	TCCR1A |= _BV(COM1B1);

	// WGM1n = 1000 : 8번 위상 및 주파수 교정 모드
	// CS1n = 010 : 분주비 8
	TCCR1B |= _BV(WGM13) | _BV(CS11);
}

void play_music(const int* pMusicNotes)
{
	int note;

	while(*pMusicNotes){
		note = *pMusicNotes;
		pMusicNotes++;

		if(note == PAUSE){
			OCR1B = 0;			// 재생 중지, 듀티 사이클 0%
		}
		else{
			// 재생 주파수 계산
			int icr_value = F_CPU / 2 / PRESCALER / note;

			// 재생 주파수 및 듀티 사이클 설정
			ICR1 = icr_value;
			OCR1B = icr_value / 2;
		}

		_delay_ms(500);			// 0.5초 간격으로 단음 재생
	}

	OCR1B = 0;				// 재생 중지
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