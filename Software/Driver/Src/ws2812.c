#include "ws2812.h"

sbit LED_PIN=P3^5;
void ws2812_writebit0(void){//24M??,1.25us??30?????
	LED_PIN=1;
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	LED_PIN=0;
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
}

void ws2812_writebit1(void){
	LED_PIN=1;
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	LED_PIN=0;
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
}

void ws2812_reset(void){//??400us??
	unsigned char i, j;
	LED_PIN=0;
	i = 13;
	j = 117;
	do
	{
		while (--j);
	} while (--i);
}

void ws2812_writecolor(uint8_t dat){//????led????
	uint8_t i;
	for( i=0;i<8;i++){
		if((dat<<i)&0x80)
			ws2812_writebit1();
		else
			ws2812_writebit0();
	}
}

void ws2812_writeled(uint8_t green,uint8_t red,uint8_t blue){//????LED???
	ws2812_writecolor(green);
	ws2812_writecolor(red);
	ws2812_writecolor(blue);
}
void ws2812_writeboard(uint8_t green,uint8_t red,uint8_t blue){
	ws2812_writeled(green,red,blue);
	ws2812_writeled(green,red,blue);
	ws2812_writeled(green,red,blue);
	ws2812_writeled(green,red,blue);
	ws2812_writeled(green,red,blue);
	ws2812_writeled(green,red,blue);
	ws2812_writeled(green,red,blue);
	ws2812_writeled(green,red,blue);
	ws2812_reset();
}

void HSVtoRGB(uint16_t h, uint16_t s, uint16_t v,uint8_t* g,uint8_t* r,uint8_t* b){
    // R,G,B from 0-255, H from 0-360, S,V from 0-100
    int i;int difs;float RGB_Adj ;
    float RGB_min, RGB_max;
    RGB_max = v*2.55f;
    RGB_min = RGB_max*(100 - s) / 100.0f;
    i = h / 60;
    difs = h % 60; // factorial part of h
    RGB_Adj = (RGB_max - RGB_min)*difs / 60.0f;
    switch (i) {
        case 0:
            *r = RGB_max;
            *g = RGB_min + RGB_Adj;
            *b = RGB_min;
            break;
        case 1:
						*r = RGB_max - RGB_Adj;
            *g = RGB_max;
            *b = RGB_min;
            break;
        case 2:
            *r = RGB_min;
            *g = RGB_max;
            *b = RGB_min + RGB_Adj;
            break;
        case 3:
            *r = RGB_min;
            *g = RGB_max - RGB_Adj;
            *b = RGB_max;
            break;
        case 4:
            *r = RGB_min + RGB_Adj;
            *g = RGB_min;
            *b = RGB_max;
            break;
        default:		// case 5:
            *r = RGB_max;
            *g = RGB_min;
            *b = RGB_max - RGB_Adj;
            break;
    }
}
