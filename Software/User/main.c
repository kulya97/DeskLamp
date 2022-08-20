#include "commond.h"
#include "ws2812.h"
#include "EEPROM.h"
#define EX2             0x10
#define EX3             0x20
#define EX4             0x40
/*************************************************/
sbit    EC11_A         =   P3^6;
sbit    EC11_B         =   P3^7;
/*************************************************/
enum TASK { LED_OFF=0, LED_WHITE, LED_COLOR ,LED_COLOR_STEP};
enum  TASK TASK_ID;
//#define LED_OFF 0;
//#define LED_WHITE 1;
//#define LED_COLOR 2;
//#define LED_COLOR_STEP 3;
/*************************************************/
uint16_t ADDR_T1_RGB=0x0400;//白光亮度地址
uint16_t ADDR_T2_HH =0x0402;//彩色H通道高8位地址
uint16_t ADDR_T2_HL =0x0403;//彩色H通道低8位地址
uint16_t ADDR_T2_RGB =0x0404;//彩色通道亮度地址
uint16_t ADDR_T3_RGB =0x0405;//渐变色亮度地址

uint8_t LED_T1_brightness;
int16_t LED_T2_color;
int8_t LED_T3_brightness;

/*************************************************/
void Delay20ms(){		//@24.000MHz
	unsigned char i, j, k;

	_nop_();
	i = 3;
	j = 112;
	k = 91;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}



void ADC_Init(){
    P1M0 = 0x00;                                //设置P1.0为ADC口
    P1M1 = 0x01;
    ADCCFG = 0x0f;                              //设置ADC时钟为系统时钟/2/16/16
    ADC_CONTR = 0x80;                           //使能ADC模块
}
void INT_Init(){
    INTCLKO = EX2;                              //使能INT2中断
	  IT1 = 1;                                    //使能INT1下降沿中断
    EX1 = 1;                                    //使能INT1中断
	  IT0 = 1;                                    //使能INT0下降沿中断
    EX0 = 1;                                    //使能INT0中断
    EA = 1;
}
void PARAMETRT_INIT(){
	LED_T1_brightness= IapRead(ADDR_T1_RGB);
	LED_T2_color= IapRead(ADDR_T2_HH)<<8|IapRead(ADDR_T2_HL);
	LED_T3_brightness= IapRead(ADDR_T3_RGB);
}
void SYS_Init(void){
	P3M0 = 0x00; 
	P3M1 = 0xc1;
	ws2812_reset();
	ADC_Init();
	INT_Init();
	PARAMETRT_INIT();
	//ws2812_writeboard(255,0,0);
}

int main(void){
	uint16_t color_h=0;
	uint8_t r,  g,  b;
	
	SYS_Init();
	while(1) {     //主循环，任务分发
		switch(TASK_ID){
			case LED_OFF:
				ws2812_writeboard(0,0,0);
				break;
			case LED_WHITE:
				ws2812_writeboard(LED_T1_brightness,LED_T1_brightness,LED_T1_brightness);
				break;
			case LED_COLOR:
				HSVtoRGB(LED_T2_color,100,LED_T1_brightness,&g,&r,&b);
				ws2812_writeboard(g,r,b);
				break;
			case LED_COLOR_STEP:
				Delay20ms();
				HSVtoRGB(color_h,100,LED_T3_brightness,&g,&r,&b);
				ws2812_writeboard(g,r,b);
				color_h++;
				if(color_h>=360)
					color_h=0;
				break;
			default:break;
		}
	}
}

void INT1_Isr() interrupt 2{//按键
		switch(TASK_ID){
			case LED_OFF:
				TASK_ID=LED_WHITE;
				break;
			case LED_WHITE:
				TASK_ID=LED_COLOR;
				break;
			case LED_COLOR:
				TASK_ID=LED_COLOR_STEP;
				break;
			case LED_COLOR_STEP:
				TASK_ID=LED_OFF;
				break;
			default:
				TASK_ID=LED_OFF;
				break;
		}
}

void INT0_Isr() interrupt 0{//EC11按键
	switch(TASK_ID){
			case LED_OFF:
				break;
			case LED_WHITE:
				IapErase(ADDR_T1_RGB);
				IapProgram(ADDR_T1_RGB, LED_T1_brightness);
				break;
			case LED_COLOR:
				IapErase(ADDR_T2_HH);
				IapProgram(ADDR_T2_HH, (LED_T2_color>>8)&0xff);
				IapErase(ADDR_T2_HL);
				IapProgram(ADDR_T2_HL, LED_T2_color&0xff);
				break;
			case LED_COLOR_STEP:
				IapErase(ADDR_T3_RGB);
				IapProgram(ADDR_T3_RGB, LED_T3_brightness);
				break;
			default:break;
		}
}
void INT2_Isr() interrupt 10{//ec11
	switch(TASK_ID){
			case LED_OFF:
				break;
			case LED_WHITE:
				if(EC11_B==1)
					LED_T1_brightness++;
				else
					LED_T1_brightness--;
				break;
			case LED_COLOR:
				if(EC11_B==1)
					LED_T2_color++;
				else
					LED_T2_color--;
				LED_T2_color=LED_T2_color<0?360:LED_T2_color;
				LED_T2_color=LED_T2_color>360?0:LED_T2_color;
				break;
			case LED_COLOR_STEP:
				if(EC11_B==1)
					LED_T3_brightness++;
				else
					LED_T3_brightness--;
				LED_T3_brightness=LED_T3_brightness<0?100:LED_T3_brightness;
				LED_T3_brightness=LED_T3_brightness>100?0:LED_T3_brightness;
				break;
			default:break;
		}

}