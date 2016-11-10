#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define LED_TIME 20

void turn_on_LED_in_PWM_manner(int dim)
{
	int i;
	
	PORTB = 0x01;				// LED 켜기
	
	for(i = 0; i < 256; i++){
		if(i > dim) PORTB = 0x00;		// 듀티 사이클을 넘어가면 LED 끄기
		_delay_us(LED_TIME);
	}
}

int main(void)
{
	DDRB = 0x01; 				// PB0 핀을 출력으로 설정
	
	int dim = 0;				// 현재 LED 밝기
	int direction = 1;			// 밝기 증가(1) 또는 감소(-1)
	
	while(1){
		turn_on_LED_in_PWM_manner(dim);

		dim += direction;			// 밝기 변화

		if(dim == 0) direction = 1;
		if(dim == 255) direction = -1;
	}

	return 0;
}