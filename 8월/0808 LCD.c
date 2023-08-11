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
#include "3.h"
#include "A1.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart2;

#define LCD_CS_HIGH();		SPIA_NCS_GPIO_Port->BSRR=SPIA_NCS_Pin
#define LCD_CS_LOW();		SPIA_NCS_GPIO_Port->BRR=SPIA_NCS_Pin
#define LCD_DCX_HIGH();		SPIA_DCX_GPIO_Port->BSRR=SPIA_DCX_Pin
#define LCD_DCX_LOW();		SPIA_DCX_GPIO_Port->BRR=SPIA_DCX_Pin
#define LCD_RST_HIGH();		DISP_NRESET_GPIO_Port->BSRR=DISP_NRESET_Pin
#define LCD_RST_LOW();		DISP_NRESET_GPIO_Port->BRR=DISP_NRESET_Pin

#define MAX_X 	319
#define MAX_Y 	239
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

#define MAX_BUFF 1000 // 25x40, 31x31, 320x3
uint8_t buff[MAX_BUFF*2];

int __io_putchar(int ch){
	(void)HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 100);
	return ch;
}

void Lcd_Write_Byte(unsigned char data){ HAL_SPI_Transmit(&hspi1, &data, 1, 10); }

void Lcd_Write_Com(unsigned char command){ LCD_DCX_LOW(); Lcd_Write_Byte(command); LCD_DCX_HIGH(); }

void Lcd_Write_Com_data(unsigned char command,unsigned char *data, int len){
	LCD_DCX_LOW(); Lcd_Write_Byte(command); LCD_DCX_HIGH();
	HAL_SPI_Transmit(&hspi1, data, len, 100);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2){
	unsigned char data[5];
	data[0]=x1>>8; data[1]=x1;data[2]=x2>>8; data[3]=x2; Lcd_Write_Com_data(0x2a,data,4);
	data[0]=y1>>8; data[1]=y1;data[2]=y2>>8; data[3]=y2; Lcd_Write_Com_data(0x2b,data,4);
	Lcd_Write_Com(0x2c);
}

void put_color(int data){
	unsigned char buff[2];
	buff[0] = data >> 8; buff[1] = data ;
	HAL_SPI_Transmit(&hspi1, buff, 2, 10);
}

void put_pixel(int x, int y, int color){
	unsigned char data[5];
	data[0]=x>>8; data[1]=x;data[2]=x>>8; data[3]=x; Lcd_Write_Com_data(0x2a,data,4);
	data[0]=y>>8; data[1]=y;data[2]=y>>8; data[3]=y; Lcd_Write_Com_data(0x2b,data,4);
	data[0]=color>>8; data[1]=color; Lcd_Write_Com_data(0x2c,data,2);
}

void box(int sX,int sY,int eX,int eY,unsigned int color){
	int i, j, k, m, c1=color>>8;
	Address_set(sX,sY,eX,eY);
	m = (eX-sX+1) * (eY-sY+1);
	k = m/MAX_BUFF; j = m%MAX_BUFF;
	if(k) for(i=0; i<MAX_BUFF; i++){ buff[2*i]=c1; buff[2*i+1]=color; }
	else for(i=0; i<j; i++){ buff[2*i]=c1; buff[2*i+1]=color; }
	while(k--){ HAL_SPI_Transmit(&hspi1, buff, MAX_BUFF*2, 10); }
	HAL_SPI_Transmit(&hspi1, buff,j*2 , 10000);
}

void LCD_Clear(int color) { box(0, 0, MAX_X, MAX_Y, color); }

void Lcd_Init(void){
	LCD_RST_HIGH(); HAL_Delay(5);
	LCD_RST_LOW(); HAL_Delay(15);
	LCD_RST_HIGH(); HAL_Delay(50);
	LCD_CS_HIGH(); LCD_CS_LOW();

	Lcd_Write_Com(0x11); HAL_Delay(120); // Exit Sleep Mode
	Lcd_Write_Com(0x3A); Lcd_Write_Byte(0x55); // Set Pixel Format 16bit
	Lcd_Write_Com(0x36); Lcd_Write_Byte(0x28); // M1=0x28, M2=0x66
	Lcd_Write_Com(0x29); // Set Display On
	LCD_Clear(BLACK);
}

void draw_eng(int COLOR){
	int row=0, col=0;
	int small=(MAX_X/9), big=(MAX_X/17);
	int change_line = (96/small), flag=1;

	for(int start=0; start<96; start++){ // 35 35 26
		if(change_line >= flag){ if(start==(small*flag)) {row=(small*flag); col+=13; flag++;} }

		for(int i=0; i<12; i++){
			for(int j=0; j<8; j++){
				if(e8x12[(start*12)+i] & (1 << (7-j))) put_pixel(j + ((start - row)*9), i+col, COLOR);
			}
		}
	}

	row=0; col=40; change_line = (96/big); flag=1;
	for(int start=0; start<96; start++){ // 18*5 6
		if(change_line >= flag){ if(start==(big*flag)) {row=(big*flag); col+=25; flag++;} }

		for(int i=0; i<24; i++){
			for(int j=0; j<16; j++){
				if(eng16x24[(start*24)+i] & (1 << (15-j))) put_pixel(j + ((start - row)*17), i+col, COLOR);
			}
		}
	}
}

void Line(int x1, int y1, int x2, int y2, int color){
	double dy=(y1-y2), dx=(x1-x2), a=0, b=0;
	if(fabs(dy) > fabs(dx)){
		a=(dx/dy); b=(x1-a*y1);
		if(y1 < y2) for(int y=y1; y<=y2; y++) put_pixel((int)(a*y+b+0.5), y, color);
		else for(int y=y2; y<=y1; y++) put_pixel((int)(a*y+b+0.5), y, color);
	} else{
		a=(dy/dx); b=(y1-a*x1);
		if(x1 < x2) for(int x=x1; x<=x2; x++) put_pixel(x, (int)(a*x+b+0.5), color);
		else for(int x=x2; x<=x1; x++) put_pixel(x, (int)(a*x+b+0.5), color);
	}
}

void draw_circle(int x, int y, int r, int angle, int color){
	for(int i=0; i<(360/angle); i++){ Line(x, y, (x + r*cos((PI/180)*i*angle)), (y + r*sin((PI/180)*i*angle)), color); }
}

int gx=0, gy=0;
const unsigned char cho[] ={0,0,0,0,0,0,0,0,1,3,3,3,1,2,4,4,4,2,1,3,0};
const unsigned char cho2[]={5,5,5,5,5,5,5,5,6,7,7,7,6,6,7,7,7,6,6,7,5};
const unsigned char jong[]={0,2,0,2,1,2,1,2,3,0,2,1,3,3,1,2,1,3,3,1,1};

int pute8X16(int ch){
	int i, k, comp, gc;

	if(ch=='\n') { gx=0; gy+=8;}
	if((ch<32) && (ch>127)) return 1;

	ch = 16 * (ch-32);
	for(k=0; k<16; k++){
		gc=e8x16[ch++]; comp=0x80;
		for(i=0; i<8; i++){
			int dispcolor = (gc & comp) ? GREEN : BLACK;
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
      int dispcolor = (chout & comp) ? GREEN : BLACK;
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
		if((ch_h >= 0xB0) && (ch_h<=0xC8)){ // ?쒓? 踰붿쐞 0xB0A1 ~ 0xC8FE
			if(in_ch > 0xA0){
				i=94*(ch_h - 0x00B0) + (in_ch - 0xA1);
				ch_kssm = KSSM[i];

				ch1 = (ch_kssm >> 10) & 0x001F; // 珥덉꽦
				ch2 = (ch_kssm >> 5) & 0x001F; // 以묒꽦
				ch3 = (ch_kssm & 0x001F); // 醫낆꽦

				if(ch1 > 1) ch1 = ch1 - 2;

				if (ch2 > 0x17) ch2 = ch2 - 9;		// 珥덉꽦&以묒꽦? 0 ?쒖옉.
				else if ( ch2 > 0x0f) ch2 = ch2 - 7;
				else if ( ch2 > 0x07) ch2 = ch2 - 5;
				else ch2 = ch2 - 3;

				if (ch3 > 0x12) ch3 = ch3 - 2;		// 醫낆꽦? 1 ?쒖옉 (?녿뒗寃쎌슦 0)
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

void pri(const char* msg){
	int index=0;
	while(1){
		if(msg[index] == '\0') break;
		if(msg[index] == '\n') {gx=0; gy+=16; index++; continue;}
		putchar(msg[index]);
		index++;
	}
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

  union bm{
	  unsigned int i;  // 3 byte
	  char c[3];
  };

  short rgbBlue, rgbGreen, rgbRed;

  if(MODE){
	  color.c[3] = bmp[position++];
	  color.c[2] = bmp[position++];
	  color.c[1] = bmp[position++];
	  color.c[0] = bmp[position++];

	  rgbBlue = ((color.i & 0xff000000) >> 24);
	  rgbGreen = ((color.i & 0x00ff0000) >> 16);
	  rgbRed = ((color.i & 0x0000ff00) >> 8);
  } else {
	  color.c[2] = bmp[position++];
	  color.c[1] = bmp[position++];
	  color.c[0] = bmp[position++];

	  rgbBlue = ((color.i & 0xff0000) >> 16);
	  rgbGreen = ((color.i & 0x00ff00) >> 8);
	  rgbRed = (color.i & 0x0000ff);
  }

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

//	draw_eng(YELLOW);
//
//	Line(10, 50, 100, 150, RED);
	draw_circle(200, 200, 30, 10, MAGENTA);
//
//	pri(str1);
//	pri("SLEEP");

	BMP_draw(a1, 0, 0, 0);
}
















