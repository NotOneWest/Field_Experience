#include "main.h"
#include "putchar.h"
#include <stdio.h>
#include <string.h>
#include "A1.h"
#include "image01.h"
#include "image02.h"
#include "image03.h"
#include "image04.h"
#include "image05.h"
extern UART_HandleTypeDef huart2;
int _write(int file, char* p, int len)
{	int i,k;
	if (pr_dir)	{HAL_UART_Transmit(&huart2,(void *) p, len, 100);}
	else
	{	switch(font_size)
		{	case 12 : for (i=0;i<len;i++) putchar_e12(*p++); break;
			case 15 : for (i=0;i<len;i++) putchar_eng8x16(*p++); break;
			case 16	: for (i=0;i<len;i++) putchar_e16(*p++); break;
			case 20	: for (i=0;i<len;i++) putchar_e20(*p++); break;
			case 24 : for (i=0;i<len;i++) putchar_e24(*p++); break;
			default : k=f_color; f_color=RED; for (i=0;i<len;i++) putchar_e24(*p++);f_color=k;break;
		}
	}
	if (cur_y > MAX_Y - 24) cur_y = 0;
	return len;
}
#define MAX_BUFF  1000  // 25x40, 31x31, 320x3
uint8_t buff[MAX_BUFF*2];
void box(int x1,int y1,int x2,int y2,int color)
{	int w,h,i,j,k,m,dy,c1=color>>8;
	w = (x2-x1+1);	h=(y2-y1+1);  m=w*h;
	j = m % MAX_BUFF;
	dy = MAX_BUFF / w;
	k = h / dy;
	if (k) for(i=0;i<MAX_BUFF;i++) { buff[2*i+1]=c1;  buff[2*i]=color; }
	else   for(i=0;i<j;i++) { buff[2*i+1]=c1;  buff[2*i]=color; }
	while(y1<=y2-dy)
	{	BSP_LCD_FillRGBRect(0,0,buff,x1,y1,w,dy);
		y1 += dy;
	}
	dy=1+y2-y1;
	BSP_LCD_FillRGBRect(0,0,buff,x1,y1,w,dy);
}
void put_pixel(int x, int y, int color)
{	buff[1]=color>>8;  buff[0]=color;
	BSP_LCD_FillRGBRect(0,0,buff,x,y,1,1);
}
void LCD_Clear(unsigned int color) {box(0,0,MAX_X,MAX_Y,color); }
void X_line(unsigned int x, unsigned int y, unsigned int x1, unsigned int color)
{	int k,i,c1=color>>8;
	uint32_t Width, Height;
	if (x>x1) { k=x; x=x1; x1=k; }
	Width=x1-x+1; Height=1;
	for (i=0;i<Width;i++) { buff[2*i+1]=c1;  buff[2*i]=color; }
	BSP_LCD_FillRGBRect(0,0,buff,x,y,Width,Height);
}
void Y_line(unsigned int x, unsigned int y, unsigned int y1, unsigned int color)
{	int k,i,c1=color>>8;;
	uint32_t Width, Height;
	if (y>y1) { k=y; y=y1; y1=k; }
	Width= 1; Height=y1-y+1;
	for (i=0;i<Height;i++) { buff[2*i+1]=c1;  buff[2*i]=color; }
	BSP_LCD_FillRGBRect(0,0,buff,x,y,Width,Height);
}
int abs(int x) { if (x>=0) return (x); else  return(-x); }
void line(int x1,int y1,int x2,int y2,int color)
{	int dx,dy,i,j,k;
	dx = x1-x2;	dy = y1-y2;
	if (abs(dx)>abs(dy))
	{	if (x1<x2)  { j = x1; k = x2; }	else  { j = x2; k = x1 ; }
		for ( i=j; i<=k ; i++ ) put_pixel(i,((i-x1)*dy)/dx + y1,color);
	} else
	{	if (y1<y2) { j = y1; k = y2; }	else  { j = y2; k = y1; }
		for ( i=j; i<=k ; i++ ) put_pixel(((i-y1)*dx)/dy + x1,i,color);
	}
}
void disp_Rect(int sX,int sY,int width,int height){ BSP_LCD_FillRGBRect(0,0,buff,sX,sY,width,height); }

unsigned long int bf4(uint8_t bmp[], int pos){
  union bm{
    unsigned int i;  // 4 byte
    char c[4];
  };

  union bm info;

  for(int k=0; k<4; k++) info.c[k]=bmp[pos++];
  return info.i;
}

int colorpalette(int position, uint8_t bmp[], int MODE){
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
	  color.c[3] = bmp[position++]; color.c[2] = bmp[position++];
	  color.c[1] = bmp[position++]; color.c[0] = bmp[position++];

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

void g071bsp(void){
	long long i;
	pr_dir=1;  // Output ==> Serial
	printf("\n\r\nProgram Start!!!\n\r");
	i=BSP_LCD_Init(0,3);  printf("BSP_LCD_Init=%d\n\r", (int) i);
	i=BSP_LCD_DisplayOn(0); printf("BSP_LCD_DisplayOn=%d\n\r", (int) i);
	LCD_Clear(BLACK);
//	font_size = 15;
	pr_dir=1;
	i=BSP_MEM_Init(0); printf("BSP_MEM_Init=%d\n\r",(int) i);
	i=BSP_MEM_EnableMemoryMappedMode(0);printf("BSP_MEM_EnableMemoryMappedMode = %d\n\r",(int) i);

	i=(int) image04;  printf("image 1 Address = %llX\n\r",i);
	BSP_MEM_ReadData(0, buff, i, 512*3);

	int imgae_len = sizeof(image04), t=(imgae_len/512), k=(imgae_len%512);
	printf("BMP LEN : %d, %d, %d\r\n", imgae_len, t, k);

	int MODE = bf4(buff, 30);
	int bfOffBits = bf4(buff, 10), biWidth = bf4(buff,18), biHeight = bf4(buff,22);
	int info_len = MODE ? ((bfOffBits/512)+1) * 512 : 512;
	printf("BMP INFO : %d, %d, %d, %d, %d\r\n", bfOffBits, MODE, biWidth, biHeight, info_len);

	uint8_t info[info_len];
	int ii=0, j=0, ix=00, iy=0, bmx=0, bmy=0, number, number2, p=0, flag=0;
	if(!MODE) { t=((imgae_len-bfOffBits)/512); k=((imgae_len-bfOffBits)%512); }

	while(1){
		if(p>0) BSP_MEM_ReadData(0, buff, i + p*512, 512*3);
		p+=3; printf("... loading ...\r\n");

		while(1){
			if(MODE==0 || MODE==3){
				if((bmx == (biWidth-1)) && (bmy == (biHeight-1))) break;
				if(bfOffBits >= (512*3)) {bfOffBits=0; break;}
				if(bmx>biWidth-1) {bmx=0; bmy++;}
				put_pixel(bmx+ix, biHeight-1-bmy+iy, (!MODE) ? colorpalette(bfOffBits, buff, MODE) : (buff[bfOffBits+1] << 8) | buff[bfOffBits]);
				bmx++; bfOffBits += (!MODE) ? 3 : 2;
			} else{
				if((bfOffBits+ii) == (512*3)) { bfOffBits=0; ii=0; flag=0; break; }

				if(flag == 2){
					for(int t=0; t<buff[bfOffBits+ii]; t++){
						if(bmx > (biWidth-1)) {bmx=0; bmy++;}
						else bmx++;
					}
					bmy += buff[bfOffBits+i+1];
					ii+=2; flag=0;
				}

				if(flag == 3){
					for(int index=0; index<(number2-j); index++){
						if(bmx>biWidth-1) {bmx=0; bmy++;}
						put_pixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(buff[bfOffBits+ii+index], info, MODE));
						bmx++;
					}
					if(number2%2==0) ii=ii+(number2-j); else ii=ii+(number2-j)+1;
					flag=0;
				}

				if(flag == 4){
					for(int c=0; c<number; c++){
						if(bmx>biWidth-1) {bmx=0;bmy++;}
						put_pixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(number2, info, MODE));
						bmx++;
					} flag=0;
				}

				if(flag == 0) number = buff[bfOffBits+i];
				if((bfOffBits+ii) == (512*3-1)) { bfOffBits=0; ii=0; flag=1; break; }

				if(number == 0x00){
					number2 = buff[(flag==1) ? bfOffBits+ii : bfOffBits+i+1];
					if(number2 == 0x00){ ii = (flag==1) ? ii+1 : ii+2; bmx=0; bmy++; flag=0; }
					if(number2 == 0x01){ ii = (flag==1) ? ii+1 : ii+2; flag=0; break;  }
					if(number2 == 0x02){
						if((bfOffBits+ii+1) == (512*3-1)) { bfOffBits=0; ii=0; flag=2; break; }

						for(int t=0; t<buff[(flag==1) ? bfOffBits+ii+1 : bfOffBits+ii+2]; t++){
							if(bmx > (biWidth-1)) {bmx=0; bmy++;}
							else bmx++;
						}
						bmy += buff[(flag==1) ? bfOffBits+ii+2 : bfOffBits+ii+3];
						i = (flag==1) ? ii+3 : ii+4; flag=0;
					}
					if(number2>=3 && number2<=255){
						for(j=0; j<number2; j++){
							if(bmx>biWidth-1) {bmx=0; bmy++;}
							int index = (flag==1) ? bfOffBits+ii+1+j : bfOffBits+ii+2+j;
							if(index == 512*3) {flag=3; break;}
							put_pixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(buff[index], info, MODE));
							bmx++;
						}
						if(flag==3) { bfOffBits=0; ii=0; break; }

						if(number2%2==0) ii=ii+number2+2; else ii=ii+number2+3;
						flag=0;
					}
				} else{
					number2 = buff[(flag==1) ? bfOffBits+ii : bfOffBits+ii+1];
					if((bfOffBits+ii+1) == (512*3-1)) { bfOffBits=0; ii=0; flag=4; break; }

					for(int c=0; c<number; c++){
						if(bmx>biWidth-1) {bmx=0;bmy++;}
						put_pixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(number2, info, MODE));
						bmx++;
					}
					ii = (flag==1) ? ii+1 : ii+2; flag=0;
				}
			}
		}
		if(p>t) break;
	}

	printf("\r--------------------DRAW--DONE--------------------------\r\n");
}
