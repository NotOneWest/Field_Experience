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
#include <math.h>
#include <stdio.h>

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim1;

#define Pin_High(Port,bit)  Port->BSRR = bit
#define Pin_Low(Port,bit)  	Port->BSRR = (bit << 16)
#define read_left()     	((left_GPIO_Port->IDR)&(left_Pin))
#define read_right()     	((right_GPIO_Port->IDR)&(right_Pin))

#define MAX_X 				479
#define MAX_Y 				319
#define PI					3.14
#define	BLACK   			0x0000
#define	BLUE    			0x001F
#define	RED     			0xF800
#define	GREEN   			0x07E0
#define CYAN    			0x07FF
#define MAGENTA 			0xF81F
#define YELLOW  			0xFFE0
#define WHITE   			0xFFFF
const int COLOR_TABLE[8] = 	{BLACK, BLUE, RED, GREEN, CYAN, MAGENTA, YELLOW ,WHITE};

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

int pute8X16(int ch){
	int i, k, comp, gc;

	if(ch=='\n') { gx=0; gy+=8;}
	if((ch<32) && (ch>127)) return 1;

	ch = 16 * (ch-32);
	for(k=0; k<16; k++){
		gc=e8x16[ch++]; comp=0x80;
		for(i=0; i<8; i++){
			int dispcolor = (gc & comp) ? YELLOW : BLACK;
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
	LCD_Clear(BLACK);

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

void pri(const char* msg){
	int index=0;
	while(1){
		if(msg[index] == '\0') break;
		if(msg[index] == '\n') {gx=0; gy+=16; index++; continue;}
		putchar(msg[index]);
		index++;
	}
}

int x=0, adv, adv_no=0, filter, diff, notch_no=0, idx=2;
long long HR, prev_RR=0, N=0, sum=0, AVE=0, square=0, SDNN=0, dif=0, RMSSD=0, condition=0;
int HR_MAX=200, HR_MIN=50, T_MAX=1200, T_MIN=300, cut=125, MAX=0;
int adv_val[16], notch[20], ADV[2]={0, }, FILTER[2], DIFF[2];

int abs(int num) {if(num < 0) num=(-num); return num;}

void msecint(){
	static int graph_count, timer_count, flag_count, adv_max, adv_min, filter_max, filter_min, diff_max, diff_min;
	HAL_ADC_Start(&hadc1);
	adv = filter = HAL_ADC_GetValue(&hadc1);

	for(int i=0; i<16; i++) filter+=adv_val[i];
	adv_no = (adv_no<15) ? adv_no+1 : 0;
	adv_val[adv_no]=adv;

	diff=abs(notch[notch_no] - filter);
	notch[notch_no]=filter;
	notch_no = (notch_no<19) ? notch_no+1 : 0;

	if(MAX < (diff/200) && ((diff/200) > cut)) { MAX=(diff/200); flag_count=0; }
	else{
		if(flag_count > T_MIN){
			prev_RR = HR;
			HR = (timer_count);
			timer_count=0; flag_count=0;
			cut = (MAX*2)/3; MAX=0; idx  = (idx+4)%8;

			N++;
			sum += HR; square += (HR*HR);
			AVE = sum/N; SDNN = sqrt(square/N-(AVE*AVE));
			if(N > 1) { dif += ((HR - prev_RR)*(HR - prev_RR)); RMSSD = sqrt(dif/(N-1)); }
			condition=(SDNN + RMSSD);
		}
	}
	if(MAX < cut) flag_count=0;
	if(timer_count > T_MAX) { timer_count=0; flag_count=0; MAX=0; cut=0;  }
	timer_count++; flag_count++;

	if(adv_max < adv) adv_max=adv;
	if(adv_min > adv) adv_min=adv;

	if(filter_max < filter) filter_max=filter;
	if(filter_min > filter) filter_min=filter;

	if(diff_max < diff) diff_max=diff;
	if(diff_min > diff) diff_min=diff;

	graph_count++;
	if(graph_count >= 15){
		ADV[0] = adv_max; ADV[1] = adv_min;
		FILTER[0] = filter_max; FILTER[1] = filter_min;
		DIFF[0] = diff_max; DIFF[1] = diff_min;

		adv_max=adv_min=adv; filter_max=filter_min=filter; diff_max=diff_min=diff;
		graph_count=0;
	}
}

void grab(){
	static int t, flag=1;
	if(!read_left() && !read_right()){
		t++;

		if(t>5000) { flag=1; msecint(); }
	} else{
		t = 0;
		x=0; adv_no=0, notch_no=0; ADV[0]=0; cut=125, MAX=0;
		prev_RR=0, N=0, sum=0, AVE=0, square=0, SDNN=0, dif=0, RMSSD=0, condition=0;
		for(int i=0; i<16; i++) adv_val[i]=0;

		if(flag){
			flag=0; LCD_Clear(BLACK);
			gx=250; gy=178; pri("HR = \0");
			gx=250; gy+=16; pri("AVE HR = \0");
			gx=250; gy+=16; pri("SDNN = \0");
			gx=250; gy+=16; pri("RMSSD = \0");
			gx=250; gy+=16; pri("Condition = \0");
			Line(0, 320, 200, 120, GREEN);
		}

//		 flag=1;
	}
}

void LineY(int x1, int y1, int y2, int color){
	int dy;
	if(y1<0) y1=0;
	if(y2<0) y2=0;
	if(y1>120) y1=120;
	if(y2>120) y2=120;
	if(y1 > y2){ dy = y1; y1=y2; y2=dy; }

	window_set(x, y1, x, y2);
	for(int y=y1; y<=y2; y++) put_color(color);
}

void lcd_main(void){
	HAL_TIM_Base_Start_IT(&htim1);
	Lcd_Init();

	gx=250; gy=178; pri("HR = \0");
	gx=250; gy+=16; pri("AVE HR = \0");
	gx=250; gy+=16; pri("SDNN = \0");
	gx=250; gy+=16; pri("RMSSD = \0");
	gx=250; gy+=16; pri("Condition = \0");
	Line(0, 320, 200, 120, GREEN);

	while(1){
		if(ADV[0]){
			x = (x<480) ? x+1 : 0;
			LineY(x, 0, 120, BLACK);

			LineY(x, (FILTER[1]/200), (FILTER[0]/200), COLOR_TABLE[idx]);
//			LineY(x, MAX_Y-(DIFF[1]/200), MAX_Y-(DIFF[0]/200), RED);
//			LineY(x, (ADV[1]*17-15000)/200, (ADV[0]*17-15000)/200, YELLOW);
//			put_pixel(x, (MAX_Y-MAX), YELLOW);
			put_pixel((60000/HR), (MAX_Y-(60000/prev_RR)), WHITE);
			put_pixel((60000/HR)+1, (MAX_Y-(60000/prev_RR)), WHITE);
			put_pixel((60000/HR), (MAX_Y-(60000/prev_RR)+1), WHITE);
			put_pixel((60000/HR)+1, (MAX_Y-(60000/prev_RR)+1), WHITE);

			if(N%15 == 1){
				gx=290; gy=178;
				pute8X16((60000/HR)/100+'0');
				pute8X16(((60000/HR)/10)%10+'0');
				pute8X16((60000/HR)%10+'0');

				gx=322; gy+=16;
				pute8X16((60000/AVE)/100+'0');
				pute8X16(((60000/AVE)/10)%10+'0');
				pute8X16((60000/AVE)%10+'0');

				gx=306; gy+=16;
				pute8X16(SDNN/100+'0');
				pute8X16((SDNN/10)%10+'0');
				pute8X16(SDNN%10+'0');

				gx=314; gy+=16;
				pute8X16(RMSSD/100+'0');
				pute8X16((RMSSD/10)%10+'0');
				pute8X16(RMSSD%10+'0');

				gx=346; gy+=16;
				pute8X16(condition/100+'0');
				pute8X16((condition/10)%10+'0');
				pute8X16(condition%10+'0');
			}
			ADV[0]=0;
		}

	}
}

















