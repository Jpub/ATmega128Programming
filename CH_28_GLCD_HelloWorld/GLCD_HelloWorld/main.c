#include <avr/io.h>
#include "u8g.h"

u8g_t u8g;

void u8g_setup(void)
{
	u8g_Init8Bit(&u8g, &u8g_dev_ks0108_128x64,
	PN(2, 0), PN(2, 1), PN(2, 2), PN(2, 3), PN(2, 4), PN(2, 5), PN(2, 6), PN(2, 7),
	PN(0, 2), PN(0, 3), PN(0, 4), PN(0, 0), PN(0, 1),
	U8G_PIN_NONE);
}

void draw(void)
{
	u8g_DrawRFrame(&u8g, 0, 0, 128, 64, 8);
	u8g_DrawLine(&u8g, 0, 0, 127, 63);
	u8g_DrawLine(&u8g, 127, 0, 0, 63);
	
	for(int r = 5; r <= 30; r+= 5){
		u8g_DrawCircle(&u8g, 64, 32, r, U8G_DRAW_ALL);
	}
}

/*
void draw(void)				// 그래픽 LCD의 그리기 함수
{
	u8g_SetFont(&u8g, u8g_font_6x10);
	u8g_DrawStr(&u8g, 0, 15, "Hello Graphic LCD!");
}
*/

int main(void)
{
	u8g_setup();

	while(1)
	{
		u8g_FirstPage(&u8g);
		do
		{
			draw();
		} while ( u8g_NextPage(&u8g) );
		u8g_Delay(100);
	}
	
	return 0;
}
