#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

// 모터를 1스텝 회전시키기 위한 데이터
uint8_t step_data[] = {0x01, 0x02, 0x04, 0x08};
int step_index = -1;			// 현재 상태

uint8_t stepForward(void)			// 시계방향 회전
{
	step_index++;
	if(step_index >= 4) step_index = 0;

	return step_data[step_index];
}

uint8_t stepBackward(void)		// 반시계방향 회전
{
	step_index--;
	if(step_index < 0) step_index = 3;
	
	return step_data[step_index];
}

int main(void)
{
	DDRC = 0x0F;				// 모터 제어 핀을 출력으로 설정
	
	while(1)
	{
		for(int i = 0; i < 200; i++){		// 200 스텝 진행
			PORTC = stepForward();		// 시계방향
			
			_delay_ms(5);			// 스텝 간격은 5ms
		}
		_delay_ms(1000);
		
		for(int i = 0; i < 200; i++){		// 200 스텝 진행
			PORTC = stepBackward();		// 반시계방향
			
			_delay_ms(5);			// 스텝 간격은 5ms
		}
		_delay_ms(1000);
	}
	
	return 0;
}