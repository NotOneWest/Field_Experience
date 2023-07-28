// 3.5 In LCD with ILI9481
#include "main.h"
#include "e8x12.h"
#include "eng16x24.h"
#include "E8x16.h"
#include "kssm.h"
#include "HAN8X16.h"
#include "HAN16.h"
#include "3.h"
#include "slamdunk.h"
#include "A3.h"
#include "crc.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart2;

#define Pin_High(Port,bit)  Port->BSRR = bit
#define Pin_Low(Port,bit)  	Port->BRR = bit
#define read_left()     	((left_GPIO_Port->IDR)&(left_Pin))
#define read_right()     	((right_GPIO_Port->IDR)&(right_Pin))
#define read_SD_DI()		((SD_DI_GPIO_Port->IDR) & (SD_DI_Pin))

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

int x=0, adv, adv_no=0, filter, diff, notch_no=0, idx=2;
long long HR, prev_RR=0, N=0, sum=0, AVE=0, square=0, SDNN=0, dif=0, RMSSD=0, condition=0;
int HR_MAX=200, HR_MIN=50, T_MAX=1200, T_MIN=300, cut=125, MAX=0;
int adv_val[16], notch[20], ADV[2]={0, }, FILTER[2], DIFF[2];

int abs(int num) {if(num < 0) num=(-num); return num;}

int __io_putchar(int ch){
	(void)HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 100);
	return ch;
}

uint8_t crc7(uint8_t *buf, int len){
	uint8_t crc=0;
	while(len--){ crc = crc7_table[(crc<<1) ^ *buf++]; }
	return crc;
}

static inline uint16_t swap16(uint16_t val){  return (((val & 0xFF00) >> 8) | ((val & 0x00FF) << 8)); }
static inline uint32_t swap32(uint32_t val){ return (((val & 0xFF000000) >> 24) | ((val & 0x00FF0000) >> 8) | ((val & 0x0000FF00) << 8) | ((val & 0x000000FF) << 24)); }

char str[30];
uint8_t outbuf[20], inbuf[512];
static uint8_t spi_read_write(uint8_t data){
	int input_data=0;
	for(int k=0x80; k>0; k>>=1){
		input_data <<= 1;
		Pin_Low(SD_SCK_GPIO_Port, SD_SCK_Pin);
		if(data & k) Pin_High(SD_DO_GPIO_Port, SD_DO_Pin); else Pin_Low(SD_DO_GPIO_Port, SD_DO_Pin);
		Pin_High(SD_SCK_GPIO_Port, SD_SCK_Pin);
		if(read_SD_DI()) input_data++;
	}
	return input_data;
}

void spi_xfer(const uint8_t *outbuf, uint8_t *inbuf, int len){
	uint8_t val;
	while(len--){
		val = spi_read_write(outbuf ? *outbuf++ : 0xFF);
		if(inbuf) *inbuf++ = val;
	}
}

void enable_cs(void) { Pin_Low(SD_SS_GPIO_Port,SD_SS_Pin); }
void disable_cs(void) { Pin_High(SD_SS_GPIO_Port,SD_SS_Pin); }
static void set_spi_mode(){ spi_xfer(NULL, NULL, 80); }

static uint8_t send_cmd(uint8_t cmd, uint32_t arg){
	uint8_t spi_buf[8], r1=0;
	enable_cs();

	spi_buf[0] = 0xFF;										// 안정적인 Data 전송 위함
	spi_buf[1] = (0x40 | (cmd & 0x3F));						// 01 + 명령어 6 bits
	spi_buf[2] = ((arg >> 24) & 0xFF);						// 8 bits argument 4개
	spi_buf[3] = ((arg >> 16) & 0xFF);
	spi_buf[4] = ((arg >> 8) & 0xFF);
	spi_buf[5] = (arg & 0xFF);
	spi_buf[6] = (((crc7(&spi_buf[1], 5) << 1) | 0x01) & 0xFF); 	// CRC 7 bits, 1
	spi_buf[7] = 0xFF;

	spi_xfer(spi_buf, NULL, 8);
	printf("CMD : ");
	for(int i=1; i<7; i++) printf("%02X ", spi_buf[i]);

	printf(" RES : ");
	for (int i=0; i<8; i++){
		spi_xfer(NULL, &r1, 1);
		printf("%02x ", r1);
		if(!(r1 & 0x80)) break;
	}
	printf("\r\n");

	disable_cs();
	return r1;
}

void SDcard_Init(){
	uint8_t r1; uint32_t data; int timeout=1000;

	disable_cs(); HAL_Delay(100);
	set_spi_mode();
	r1=send_cmd(0, 0); sprintf(str,"Set Idle(CMD0): %s\n", r1 == 0x01 ? "PASS" : "FAIL"); pri(str);
	r1=send_cmd(8, 0x100);
	sprintf(str,"SEND_IF_COND: "); pri(str);
	if (r1 == 0x01){
		enable_cs(); spi_xfer(NULL, (void*)&data, 4); disable_cs();
		sprintf(str,"%08lX\n", swap32(data)); pri(str);
	} else {sprintf(str,"FAIL\n"); pri(str); }

	sprintf(str,"SEND_OP_COND: "); pri(str);
	while(1){
		r1=send_cmd(55, 0);
		if (r1 != 0x01) { sprintf(str,"FAIL\n"); pri(str); break; }
		r1=send_cmd(41, 0x40000000);
		if (r1 == 0x00) { sprintf(str,"PASS\n"); pri(str); break; }
		if(timeout-- <= 0) { sprintf(str,"Time out\n"); pri(str); break; }
	}

	r1=send_cmd(58, 0);
	sprintf(str,"READ OCR: "); pri(str);
	if (!(r1 & 0xFE)){
		enable_cs(); spi_xfer(NULL, (void*)&data, 4); disable_cs();
		sprintf(str,"%08lX\n", swap32(data)); pri(str);
	} else {sprintf(str,"FAIL\n"); pri(str); }
}

void read_data(uint8_t *read_buf, uint32_t addr){
	uint8_t r1=0; uint32_t data=0, TIMEOUT=0;

	r1 = send_cmd(17, addr);
	if(r1 != 0x00){ printf("Read Command Fail[0x%02lX]\r\n", addr); return; }
	enable_cs();
	while(1){
		spi_xfer(NULL, &r1, 1);  TIMEOUT++;
		if(r1 == 0xFE) { printf("\r\nRead Single Block[0x%02lX]\r\n", addr); break; }
		if(r1 == 0xF0) { printf("Address Error\r\n"); return; }
		if(TIMEOUT > 1000) { printf("Time Out\r\n"); return; }
	}

	for(int i=0; i<128; i++){
		if(i%4 == 0) { printf("\r\n%02X: ", 0+(i/4));}
		spi_xfer(NULL, read_buf+(i*4), 4);
		data = (read_buf[(i*4)]<<24) | (read_buf[(i*4)+1]<<16) | (read_buf[(i*4)+2]<<8) | (read_buf[(i*4)+3]);
		printf("%08lX ", data);
	}
	spi_xfer(NULL, &r1, 1); spi_xfer(NULL, &r1, 1);
	disable_cs();
	printf("\r----------------------------------------------------\r\n");
}

void write_data(uint8_t *outbuf, uint32_t addr){
	uint8_t r1; int attempt=3907;

	r1 = send_cmd(24, addr);
	if(r1 == 0xFF){ printf("Write Command Fail\r\n"); return; }
	printf("\r\nWrite Single Block[0x%02lX]\r\n", addr);

	enable_cs();
	r1=0xFE; spi_xfer(&(r1), NULL, 1);
	spi_xfer(outbuf, NULL, 512);

	while(attempt--){
		spi_xfer(NULL, &(r1), 1);
		if(r1 != 0xFF) break;
	}

	if((r1 & 0x1F) == 0x05){
		attempt = 3907;
		while(attempt--){
			spi_xfer(NULL, &(r1), 1);
			if(r1 != 0x00) break;
		}
	}

	disable_cs();
	printf("\r---------------------------------------------------\r\n");
}

void write_BMP(const char BMP[], int len, int t, int k){
	uint8_t write_buf[512];

	for(int j=0; j<=t; j++){
		for(int i=0; i<512; i++){
			if(j==t){
				if(i<k) write_buf[i] = BMP[i+j*512];
				else write_buf[i] = 0;
			} else write_buf[i] = BMP[i+j*512];
		}
		write_data(write_buf, j*512);
	}
	printf("\r----------------------WRITE--DONE--------------------------\r\n");
}

void lcd_main(void){
//	HAL_TIM_Base_Start_IT(&htim1);
	Lcd_Init();
	SDcard_Init();
	SDcard_Init();
	printf("\r----------------------INIT--DONE--------------------------\r\n");

	int len = sizeof(a3), t=(len/512), k=(len%512);
	printf("BMP LEN : %d, %d, %d", len, t, k);

	write_BMP(a3, len, t, k);

	uint8_t read_buf[512*3];
	read_data(read_buf, 0);
	int bfOffBits = bf4((const char *)read_buf, 10);	//비트맵 데이터의 시작 위치
	int MODE = bf4((const char *)read_buf, 30);
	int biWidth = bf4((const char *)read_buf,18), biHeight = bf4((const char *)read_buf,22);
	int info_len = MODE ? ((bfOffBits/512)+1) * 512 : 512;

	uint8_t info[info_len];
	for(int j=0; j<(info_len/512); j++) read_data(info+j*512, j*512);
	printf("\r--------------------INFO-READ--DONE--------------------------\r\n");

	int i=0, j=0, ix=0, iy=0, bmx=0, bmy=0, number, number2, p=0, flag=0;
	if(!MODE) { t=((len-bfOffBits)/512); k=((len-bfOffBits)%512); }

	while(1){
		if(p<=t) { read_data(read_buf, p*512); p++; }
		if(p<=t) { read_data(read_buf+512, p*512); p++; }
		if(p<=t) { read_data(read_buf+1024, p*512); p++; }

		while(1){
			if(MODE==0){
				if((bmx == biWidth) && (bmy == biHeight)) break;
				if(bfOffBits >= (512*3)) {bfOffBits=0; break;}
				if(bmx>biWidth-1) {bmx=0; bmy++;}
				put_pixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(bfOffBits, (const char *)read_buf, MODE));
				bmx++; bfOffBits+=3;
			} else{
				if((bfOffBits+i) == (512*3)) { bfOffBits=0; i=0; flag=0; break; }

				if(flag == 2){
					for(int t=0; t<read_buf[bfOffBits+i]; t++){
						if(bmx > (biWidth-1)) {bmx=0; bmy++;}
						else bmx++;
					}
					bmy += read_buf[bfOffBits+i+1];
					i+=2; flag=0;
				}

				if(flag == 3){
					for(int index=0; index<(number2-j); index++){
						if(bmx>biWidth-1) {bmx=0; bmy++;}
						put_pixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(read_buf[bfOffBits+i+index], (const char *)info, MODE));
						bmx++;
					}
					if(number2%2==0) i=i+(number2-j); else i=i+(number2-j)+1;
					flag=0;
				}

				if(flag == 4){
					for(int c=0; c<number; c++){
						if(bmx>biWidth-1) {bmx=0;bmy++;}
						put_pixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(number2, (const char *)info, MODE));
						bmx++;
					} flag=0;
				}

				if(flag == 0) number = read_buf[bfOffBits+i];
				if((bfOffBits+i) == (512*3-1)) { bfOffBits=0; i=0; flag=1; break; }

				if(number == 0x00){
					number2 = read_buf[(flag==1) ? bfOffBits+i : bfOffBits+i+1];
					if(number2 == 0x00){ i = (flag==1) ? i+1 : i+2; bmx=0; bmy++; flag=0; }
					if(number2 == 0x01){ i = (flag==1) ? i+1 : i+2; flag=0; break;  }
					if(number2 == 0x02){
						if((bfOffBits+i+1) == (512*3-1)) { bfOffBits=0; i=0; flag=2; break; }

						for(int t=0; t<read_buf[(flag==1) ? bfOffBits+i+1 : bfOffBits+i+2]; t++){
							if(bmx > (biWidth-1)) {bmx=0; bmy++;}
							else bmx++;
						}
						bmy += read_buf[(flag==1) ? bfOffBits+i+2 : bfOffBits+i+3];
						i = (flag==1) ? i+3 : i+4; flag=0;
					}
					if(number2>=3 && number2<=255){

						for(j=0; j<number2; j++){
							if(bmx>biWidth-1) {bmx=0; bmy++;}
							int index = (flag==1) ? bfOffBits+i+1+j : bfOffBits+i+2+j;
							if(index == 512*3) {flag=3; break;}
							put_pixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(read_buf[index], (const char *)info, MODE));
							bmx++;
						}
						if(flag==3) { bfOffBits=0; i=0; break; }

						if(number2%2==0) i=i+number2+2; else i=i+number2+3;
						flag=0;
					}
				} else{
					number2 = read_buf[(flag==1) ? bfOffBits+i : bfOffBits+i+1];
					if((bfOffBits+i+1) == (512*3-1)) { bfOffBits=0; i=0; flag=4; break; }

					for(int c=0; c<number; c++){
						if(bmx>biWidth-1) {bmx=0;bmy++;}
					    put_pixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(number2, (const char *)info, MODE));
					    bmx++;
					}
					i = (flag==1) ? i+1 : i+2; flag=0;
				}
			}
		}
		if(p>t) break;
	}
	printf("\r----------------------DRAW--DONE--------------------------\r\n");
}
