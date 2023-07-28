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
#include "A1.h"
#include "A3.h"
#include "3.h"
#include "aaaa.h"
#include "slamdunk.h"
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
	LCD_Clear(WHITE);

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

unsigned long int bf4(const char bmp[], int pos){
  union bm{
    unsigned int i;  // 4 byte
    char c[4];
  };
  union bm info;

  for(int k=0; k<4; k++) info.c[k]=bmp[pos++];
  return info.i;
}

int colorpalette(int position, const char bmp[], int MODE){
  if(MODE) position = position*4+54;

  union palette{
    unsigned int i;
    char c[4];
  };
  union palette color;

  if(MODE) color.c[3] = bmp[position++];
  else color.c[3] = 0x00;
  color.c[2] = bmp[position++];
  color.c[1] = bmp[position++];
  color.c[0] = bmp[position++];

  short rgbBlue = ((color.i & 0xff000000) >> 24);
  short rgbGreen = ((color.i & 0x00ff0000) >> 16);
  short rgbRed = ((color.i & 0x0000ff00) >> 8);

  rgbRed = rgbRed >> 3;
  rgbGreen = rgbGreen >> 2;
  rgbBlue = rgbBlue >> 3;

  int colord = ((rgbRed<<11)|(rgbGreen<<5)|(rgbBlue));
  return colord;
}


void BMP_draw(const char bmp[], int ix, int iy, int MODE){
  int i=0, j=0, bmx=0, bmy=0, number, number2;
  int bfOffBits = bf4(bmp,10);	//비트맵 데이터의 시작 위치
  int biWidth = bf4(bmp,18);    //480-1 // 400-1
  int biHeight = bf4(bmp,22);   //272-1 // 300-1

  while(1){
	  if(MODE==0){
		  if((bmx == biWidth) && (bmy == biHeight)) break;
		  if(bmx>biWidth-1) {bmx=0; bmy++;}
		  put_pixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(bfOffBits, bmp, MODE));
		  bmx++; bfOffBits+=3;
	  } else{
		  number = bmp[bfOffBits+i];
		  if(number == 0x00){
			number2 = bmp[bfOffBits+i+1];
			if(number2 == 0x00){i+=2; bmx=0; bmy++;}
			if(number2 == 0x01){i+=2;break;}
			if(number2 == 0x02){
			  for(int t=0; t<bmp[bfOffBits+i+2]; t++){
				  if(bmx > (biWidth-1)) {bmx=0; bmy++;}
				  else bmx++;
			  }
			  bmy += bmp[bfOffBits+i+3];
			  i+=4;
			}
			if(number2>=3 && number2<=255){
			  for(j=0; j<number2; j++){
				if(bmx>biWidth-1) {bmx=0; bmy++;}
				put_pixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(bmp[bfOffBits+i+2+j], bmp, MODE));
				bmx++;
			  }
			  if(number2%2==0) i=i+number2+2; else i=i+number2+3;
			}
		  } else{
			number2 = bmp[bfOffBits+i+1];
			for(j=0; j<number; j++){
			  if(bmx>biWidth-1) {bmx=0;bmy++;}
			  put_pixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(bmp[bfOffBits+i+1], bmp, MODE));
			  bmx++;
			}
			i+=2;
		  }
	  }
  }
}

void lcd_main(void){
	Lcd_Init();

	BMP_draw(a1, 120, 92, 0);
//	BMP_draw(slamdunk, 150, 0, 0);
//	BMP_draw(a3, 0, 0, 1);
//	BMP_draw(three, 40, 10, 1);
}

















