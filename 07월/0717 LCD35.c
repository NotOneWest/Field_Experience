// 3.5 In LCD with ILI9481
#include "main.h"
#include "e8x12.h"
#include "eng16x24.h"
#include <math.h>

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

int db[400]={0, };
void Set_Font(int mode, int start, int BC, int FC){
	for(int y=0; y<12*(mode); y++){
		for(int x=0; x<(8*mode); x++){
			if(mode==2){
				if(eng16x24[start+y] & (1 << ((8*mode-1)-x))) db[x+y*16]=FC;
				else db[x+y*16]=BC;
			} else{
				if(e8x12[start+y] & (1 << ((8*mode-1)-x))) db[x+y*8]=FC;
				else db[x+y*8]=BC;
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
	int maxAngle=(90/angle);
	Line(x, y, x, y-r, color);
	for(int i=1; i<maxAngle; i++) Line(x, y, x+(r*sin(((angle*i) * (PI/180)))), y-(r*cos(((angle*i) * (PI/180)))), color);
	Line(x, y, x+r, y, color);
	for(int i=1; i<maxAngle; i++) Line(x, y, x+(r*cos(((angle*i) * (PI/180)))), y+(r*sin(((angle*i) * (PI/180)))), color);
	Line(x, y, x, y+r, color);
	for(int i=1; i<maxAngle; i++) Line(x, y, x-(r*sin(((angle*i) * (PI/180)))), y+(r*cos(((angle*i) * (PI/180)))), color);
	Line(x, y, x-r, y, color);
	for(int i=1; i<maxAngle; i++) Line(x, y, x-(r*cos(((angle*i) * (PI/180)))), y-(r*sin(((angle*i) * (PI/180)))), color);
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
	LCD_Clear(BLUE);

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

void lcd_main(void){
	Lcd_Init();
	
//	int row=0, col=0;
//	for(int start=0; start<96; start++){
//		if((start*12) > 635) {row=53; col=13;}
//		for(int i=0; i<12; i++){
//			for(int j=0; j<8; j++){
//				if(e8x12[(start*12)+i] & (1 << (7-j))) put_pixel(j + ((start - row)*9), i+col, RED);
//			}
//		}
//	}
//
//	row=0; col=26;
//	for(int start=0; start<96; start++){
//		if((start*24)==672) {row=28; col+=25;}
//		if((start*24)==1344) {row=28*2; col+=25;}
//		if((start*24)==2016) {row=28*3; col+=25;}
//		for(int i=0; i<24; i++){
//			for(int j=0; j<16; j++){
//				if(eng16x24[(start*12)+i] & (1 << (15-j))) put_pixel(j + ((start - row)*17), i+col, RED);
//			}
//		}
//	}

	int cx=0, cy=0;
	for(int start=0; start<96; start++){
		Set_Font(1, start*12, BLUE, YELLOW);
		if((start*12)==636) cy+=13;
		window_set(cx, cy, cx+7, cy+11);
		for(int y=0; y<12; y++){
			for(int x=0; x<8; x++) put_color(db[x+y*8]);
		}
		cx = (cx<470) ? cx+9 : 0;
	}

	cx=0; cy+=13;
	for(int start=0; start<96; start++){
		Set_Font(2, start*24, BLUE, YELLOW);
		if((start*24)==672) {cx=0; cy+=25;}
		if((start*24)==1344) {cx=0; cy+=25;}
		if((start*24)==2016) {cx=0; cy+=25;}
		window_set(cx, cy, cx+15, cy+23);
		for(int y=0; y<24; y++){
			for(int x=0; x<16; x++) put_color(db[x+y*16]);
		}
		cx = (cx<462) ? cx+17 : 0;
	}

	Line(100, 200, 150, 150, MAGENTA);
	draw_circle(300, 210, 100, 10, MAGENTA);
}

















