// 3.5 In LCD with ILI9481
#include "main.h"
#include "e8x12.h"
#include "eng16x24.h"
#include "E8x16.h"
#include "kssm.h"
#include "HAN8X16.h"
#include "HAN16.h"
#include "song.h"
#include "for.h"
#include "straw.h"
#include <math.h>
#include <stdio.h>

#define Pin_High(Port,bit)  Port->BSRR = bit
#define Pin_Low(Port,bit)  Port->BSRR = (bit << 16)
#define MAX_X 	479
#define MAX_Y 	319
#define PI		3.14
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
const int COLOR_TABLE[8] = {BLACK, BLUE, RED, GREEN, CYAN, MAGENTA, YELLOW ,WHITE};

int db[400]={0, }, gx=0, gy=0;
const unsigned char cho[] ={0,0,0,0,0,0,0,0,1,3,3,3,1,2,4,4,4,2,1,3,0};
const unsigned char cho2[]={5,5,5,5,5,5,5,5,6,7,7,7,6,6,7,7,7,6,6,7,5};
const unsigned char jong[]={0,2,0,2,1,2,1,2,3,0,2,1,3,3,1,2,1,3,3,1,1};

void Lcd_Write_Byte(int d){
	Pin_Low(LCD_WR_GPIO_Port,LCD_WR_Pin);
	if (d & 0x01) Pin_High(LCD_D0_GPIO_Port,LCD_D0_Pin); else  Pin_Low(LCD_D0_GPIO_Port,LCD_D0_Pin);
	if (d & 0x02) Pin_High(LCD_D1_GPIO_Port,LCD_D1_Pin); else  Pin_Low(LCD_D1_GPIO_Port,LCD_D1_Pin);
	if (d & 0x04) Pin_High(LCD_D2_GPIO_Port,LCD_D2_Pin); else  Pin_Low(LCD_D2_GPIO_Port,LCD_D2_Pin);
	if (d & 0x08) Pin_High(LCD_D3_GPIO_Port,LCD_D3_Pin); else  Pin_Low(LCD_D3_GPIO_Port,LCD_D3_Pin);
	if (d & 0x10) Pin_High(LCD_D4_GPIO_Port,LCD_D4_Pin); else  Pin_Low(LCD_D4_GPIO_Port,LCD_D4_Pin);
	if (d & 0x20) Pin_High(LCD_D5_GPIO_Port,LCD_D5_Pin); else  Pin_Low(LCD_D5_GPIO_Port,LCD_D5_Pin);
	if (d & 0x40) Pin_High(LCD_D6_GPIO_Port,LCD_D6_Pin); else  Pin_Low(LCD_D6_GPIO_Port,LCD_D6_Pin);
	if (d & 0x80) Pin_High(LCD_D7_GPIO_Port,LCD_D7_Pin); else  Pin_Low(LCD_D7_GPIO_Port,LCD_D7_Pin);
	Pin_High(LCD_WR_GPIO_Port, LCD_WR_Pin);
}

void Lcd_Write_Com(int comm)  {
	Pin_Low(LCD_RS_GPIO_Port,LCD_RS_Pin);
	Lcd_Write_Byte(comm);
	Pin_High(LCD_RS_GPIO_Port,LCD_RS_Pin);
}

void window_set(int x1,int y1,int x2,int y2){
	Lcd_Write_Com(0x2a); 	Lcd_Write_Byte(x1>>8);	Lcd_Write_Byte(x1);	Lcd_Write_Byte(x2>>8);	Lcd_Write_Byte(x2);
    Lcd_Write_Com(0x2b);	Lcd_Write_Byte(y1>>8);	Lcd_Write_Byte(y1);	Lcd_Write_Byte(y2>>8);	Lcd_Write_Byte(y2);
	Lcd_Write_Com(0x2c); 							 
}

void put_color(int color) { Lcd_Write_Byte(color>>8); Lcd_Write_Byte(color); }
void put_pixel(int x, int y, int color){
	unsigned char x1=x>>8,y1=y>>8,x2=x&0xff,y2=y&0xff;
	Lcd_Write_Com(0x2a); 	Lcd_Write_Byte(x1);	Lcd_Write_Byte(x2);	Lcd_Write_Byte(x1);	Lcd_Write_Byte(x2);
	Lcd_Write_Com(0x2b);	Lcd_Write_Byte(y1);	Lcd_Write_Byte(y2);	Lcd_Write_Byte(y1);	Lcd_Write_Byte(y2);
	Lcd_Write_Com(0x2c);	Lcd_Write_Byte(color>>8);	Lcd_Write_Byte(color);
}

void LCD_Clear(unsigned int color){
	int ci,cj;
	int m;
	window_set(0,0,MAX_X,MAX_Y);
	ci = color >> 8; cj = color & 0xff;
	m = (MAX_X+1) * (MAX_Y+1);
	for(;m>0;m--)  { Lcd_Write_Byte(ci); Lcd_Write_Byte(cj); }
}

int pute8X16(int ch){
	int i, k, comp, gc;

	if(ch=='\n') { gx=0; gy+=8;}
	if((ch<32) && (ch>127)) return 1;

	ch = 16 * (ch-32);
	for(k=0; k<16; k++){
		gc=e8x16[ch++]; comp=0x80;
		for(i=0; i<8; i++){
			int dispcolor = (gc & comp) ? BLACK : GREEN;
			put_pixel(gx+i, gy+k, dispcolor);
			comp = comp >> 1;
		}
	}
	if(gx+16<MAX_X) {gx+=8;} else {gx=0; gy=gy+16;}
	return ch;
}

void puthan16(unsigned short int ch1, unsigned short int ch2, unsigned short int ch3){
  unsigned int i, k, comp, chout;
  union inch{
    short unsigned int i;
    char c[2];
  };
  union inch gch1, gch2, gch3;

  ch1 = 32 * ch1; ch2 = 32 * ch2; ch3 = 32 * ch3;

  for(k=0; k<16 ; k++){
    gch1.c[1] = han16[ch1++]; gch1.c[0] = han16[ch1++];
    gch2.c[1] = han16[ch2++]; gch2.c[0] = han16[ch2++];

    if((ch3)){
      gch3.c[1] = han16[ch3++]; gch3.c[0] = han16[ch3++];
      chout = gch1.i | gch2.i | gch3.i ;
    } else chout = gch1.i | gch2.i ;

    comp = 0x8000;
    for(i=0; i<16; i++){
      int dispcolor = (chout & comp) ? BLACK : GREEN;
      put_pixel(gx+i, gy+k, dispcolor);
      comp = comp >> 1;
    }
  }
  if(gx+32<MAX_X) { gx = gx + 16; }  else { gx=0; gy = gy + 16; }
}

int putchar(int ch){
	unsigned short int in_ch, ch_kssm, i, j, k, ch1, ch2, ch3;
	static unsigned short int ch_h=0;

	in_ch = 0x00ff & ch;

	if(ch_h){
		if((ch_h >= 0xB0) && (ch_h<=0xC8)){ // 한글 범위 0xB0A1 ~ 0xC8FE
			if(in_ch > 0xA0){
				i=94*(ch_h - 0x00B0) + (in_ch - 0xA1);
				ch_kssm = KSSM[i];

				ch1 = (ch_kssm >> 10) & 0x001F; // 초성
				ch2 = (ch_kssm >> 5) & 0x001F; // 중성
				ch3 = (ch_kssm & 0x001F); // 종성

				if(ch1 > 1) ch1 = ch1 - 2;

				if (ch2 > 0x17) ch2 = ch2 - 9;		// 초성&중성은 0 시작.
				else if ( ch2 > 0x0f) ch2 = ch2 - 7;
				else if ( ch2 > 0x07) ch2 = ch2 - 5;
				else ch2 = ch2 - 3;

				if (ch3 > 0x12) ch3 = ch3 - 2;		// 종성은 1 시작 (없는경우 0)
				else if ( ch3 > 1) ch3 = ch3 - 1;
				else ch3 = 0;

				if(ch3 == 0){
					i = cho[ch2];
					if(ch1 == 0 || ch1 == 15) j=0;
					else j=1;
				} else {
					i=cho2[ch2];
					if(ch1 == 0 || ch1 == 15) j=2;
					else j=3;
					k=jong[ch2];
				}

				ch1 = (i*19) + ch1;
				ch2 = (8*19) + (j*21) + ch2;
				if(ch3) ch3 = (8*19) + (4*21) + (k*27) + ch3-1;

				puthan16(ch1, ch2, ch3);
			}
		}
		ch_h=0;
	} else {if(in_ch < 0x80) {pute8X16(ch);} else ch_h = in_ch;}
	return(ch);
}

void Lcd_Init(void){

	Pin_High(LCD_RD_GPIO_Port,LCD_RD_Pin);
	Pin_High(LCD_RST_GPIO_Port,LCD_RST_Pin);	HAL_Delay(5);
	Pin_Low(LCD_RST_GPIO_Port,LCD_RST_Pin);		HAL_Delay(15);
	Pin_High(LCD_RST_GPIO_Port,LCD_RST_Pin);	HAL_Delay(50);
	Pin_High(LCD_CS_GPIO_Port,LCD_CS_Pin);
	Pin_High(LCD_WR_GPIO_Port,LCD_WR_Pin);
	Pin_Low(LCD_CS_GPIO_Port,LCD_CS_Pin);
	Lcd_Write_Com(0x11);						HAL_Delay(120);	// Exit Sleep Mode
	Lcd_Write_Com(0x3A);	Lcd_Write_Byte(0x55);		// Set Pixel Format 16bit
	Lcd_Write_Com(0x36);	Lcd_Write_Byte(0x28);		// Set Address Mode  BGR Order
	Lcd_Write_Com(0x29);		// Set Display On
	LCD_Clear(GREEN);

		// Positive Gamma Control
	Lcd_Write_Com(0xE0);
	Lcd_Write_Byte(0x0F);
	Lcd_Write_Byte(0x1B);
	Lcd_Write_Byte(0x18);
	Lcd_Write_Byte(0x0B);
	Lcd_Write_Byte(0x0E);
	Lcd_Write_Byte(0x09);
	Lcd_Write_Byte(0x47);
	Lcd_Write_Byte(0x94);
	Lcd_Write_Byte(0x35);
	Lcd_Write_Byte(0x0A);
	Lcd_Write_Byte(0x13);
	Lcd_Write_Byte(0x05);
	Lcd_Write_Byte(0x08);
	Lcd_Write_Byte(0x03);
	Lcd_Write_Byte(0x00);

	// Negative Gamma Correction
	Lcd_Write_Com(0XE1);
	Lcd_Write_Byte(0x0F);
	Lcd_Write_Byte(0x3A);
	Lcd_Write_Byte(0x37);
	Lcd_Write_Byte(0x0B);
	Lcd_Write_Byte(0x0C);
	Lcd_Write_Byte(0x05);
	Lcd_Write_Byte(0x4A);
	Lcd_Write_Byte(0x24);
	Lcd_Write_Byte(0x39);
	Lcd_Write_Byte(0x07);
	Lcd_Write_Byte(0x10);
	Lcd_Write_Byte(0x04);
	Lcd_Write_Byte(0x27);
	Lcd_Write_Byte(0x25);
	Lcd_Write_Byte(0x00);
}

void pri(){
	int index=0;
	while(1){
		if(straw[index] == '\0') break;
		putchar(straw[index]);
		index++;
	}
}

void lcd_main(void){
	Lcd_Init();
	pri();
}

















