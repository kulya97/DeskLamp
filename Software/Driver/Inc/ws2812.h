#ifndef __WS2812_H_
#define __WS2812_H_

#include	"commond.h"
void HSVtoRGB(uint16_t h, uint16_t s, uint16_t v,uint8_t* g,uint8_t* r,uint8_t* b);
void ws2812_writebit0(void);
void ws2812_writebit1(void);
void ws2812_reset(void);//??
void ws2812_writecolor(uint8_t brightness);//????led????
void ws2812_writeled(uint8_t green,uint8_t red,uint8_t blue);//????LED???
void ws2812_writeboard(uint8_t green,uint8_t red,uint8_t blue);

#endif
