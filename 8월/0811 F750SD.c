#include "main.h"
#include "stm32f7xx_hal.h"
#include <stdlib.h>
#include "stm32f7508_discovery.h"
#include "stm32f7508_discovery_lcd.h"
#include <stdio.h>
#include "stm32f7508_discovery_qspi.h"
#include "fonts.h"
#include "ff.h"
#include <math.h>
extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc3;
extern TIM_HandleTypeDef htim2;

#define  MAX_Y       271

#define  BLACK       0x0000      /*   0,   0,   0 */
#define  NAVY        0x000F      /*   0,   0, 128 */
#define  DARKGREEN   0x03E0      /*   0, 128,   0 */
#define  DARKCYAN    0x03EF      /*   0, 128, 128 */
#define  MAROON      0x7800      /* 128,   0,   0 */
#define  PURPLE      0x780F      /* 128,   0, 128 */
#define  OLIVE       0x7BE0      /* 128, 128,   0 */
#define  LIGHTGREY   0xC618      /* 192, 192, 192 */
#define  DARKGREY    0x7BEF      /* 128, 128, 128 */
#define  BLUE        0x001F      /*   0,   0, 255 */
#define  GREEN       0x07E0      /*   0, 255,   0 */
#define  CYAN        0x07FF      /*   0, 255, 255 */
#define  RED         0xF800      /* 255,   0,   0 */
#define  MAGENTA     0xF81F      /* 255,   0, 255 */
#define  YELLOW      0xFFE0      /* 255, 255,   0 */
#define  WHITE       0xFFFF      /* 255, 255, 255 */
#define  ORANGE      0xFD20      /* 255, 165,   0 */
#define  GREENYELLOW 0xAFE5      /* 173, 255,  47 */
const unsigned short int  COLOR_TABLE[16] = {  BLACK, BLUE, GREEN, RED, NAVY, DARKGREEN, DARKCYAN,
						MAROON, PURPLE, OLIVE, LIGHTGREY, DARKGREY, CYAN, MAGENTA, ORANGE, GREENYELLOW};

unsigned short int *Buffer = (unsigned short int *)0xC0000000;
#define putPixel(x,y,color) Buffer[x+y*480]=color

int _write(int file, uint8_t *p, int len)
{	HAL_UART_Transmit(&huart1, p, len, 100);
   return len;
}

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
	  position = position*4+54;
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

FATFS SDFatFs; /* File system object for SD card logical drive */
FIL MyFile; /* File object */
FRESULT res;
UINT* br;

uint8_t buff[1536];
void f750sd(void){
	int x=0;
	BSP_LED_Init(0);
	BSP_LCD_Init();
	BSP_LCD_Init();
	BSP_LCD_LayerRgb565Init(0, LCD_FB_START_ADDRESS);
	BSP_LCD_DisplayOn();
    BSP_LCD_SelectLayer(0);
    BSP_LCD_Clear(BLACK);
    x=BSP_QSPI_Init ();
    printf("\n\rBSP_QSPI_Init = %d\n\r",x);
    x= BSP_QSPI_EnableMemoryMappedMode();
    printf("\n\rEnableMemoryMapped = %d\n\r",x);

    res = f_mount(&SDFatFs, "", 0);
    if(res != 0) printf("MOUNT ERROR : %d\r\n", res);

    res = f_open(&MyFile, "a2.bmp", FA_READ);
    if(res != 0) printf("OPEN ERROR : %d\r\n", res);

    f_read(&MyFile, buff, 1536, br);
    int image_len = bf4(buff, 2), t=(image_len/512), k=(image_len%512);
    printf("BMP LEN : %d, %d, %d\r\n", image_len, t, k);

    int MODE = bf4(buff, 30), palette_num = bf4(buff, 46);
    int bfOffBits = bf4(buff, 10), biWidth = bf4(buff,18), biHeight = bf4(buff,22);
    int info_len = (palette_num) ? ((bfOffBits/512)+1) * 512 : 512;
	printf("BMP INFO : %d, %d, %d, %d, %d\r\n", bfOffBits, MODE, biWidth, biHeight, info_len);

	uint8_t info[info_len];
	int i=0, j=0, ix=0, iy=0, bmx=0, bmy=0, number=0, number2=0, p=0, flag=0;
	if(palette_num) { t=((image_len-bfOffBits)/512); k=((image_len-bfOffBits)%512); }
	f_lseek(&MyFile, 0); f_read(&MyFile, info, info_len, br);

	while(1){
		if(p>0) {f_lseek(&MyFile, 512*p); f_read(&MyFile, buff, 1536, br);}
		p+=3; printf("... loading ...\r\n");
		if(br != FR_OK) printf("---error---\r\n");

		while(1){
			if(MODE==0 || MODE==3){
				if((bmx == (biWidth-1)) && (bmy == (biHeight-1))) break;
				if(bfOffBits >= (512*3)) {bfOffBits=0; break;}
				if(bmx>biWidth-1) {bmx=0; bmy++;}
				if(palette_num){ // a2.bmp
					BSP_LCD_DrawPixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(buff[bfOffBits], info, 1));
					bmx++; bfOffBits++;
				} else{
					BSP_LCD_DrawPixel(bmx+ix, biHeight-1-bmy+iy, (!MODE) ? colorpalette(bfOffBits, buff, MODE) : (buff[bfOffBits+1] << 8) | buff[bfOffBits]);
					bmx++; bfOffBits += (!MODE) ? 3 : 2;
				}
			} else{
				if((bfOffBits+i) == (512*3)) { bfOffBits=0; i=0; flag=0; break; }

				if(flag == 2){
					for(int t=0; t<buff[bfOffBits+i]; t++){
						if(bmx > (biWidth-1)) {bmx=0; bmy++;}
						else bmx++;
					}
					bmy += buff[bfOffBits+i+1];
					i+=2; flag=0;
				}

				if(flag == 3){
					for(int index=0; index<(number2-j); index++){
						if(bmx>biWidth-1) {bmx=0; bmy++;}
						BSP_LCD_DrawPixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(buff[bfOffBits+i+index], info, MODE));
						bmx++;
					}
					if(number2%2==0) i=i+(number2-j); else i=i+(number2-j)+1;
					flag=0;
				}

				if(flag == 4){
					for(int c=0; c<number; c++){
						if(bmx>biWidth-1) {bmx=0;bmy++;}
						BSP_LCD_DrawPixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(number2, info, MODE));
						bmx++;
					} flag=0;
				}

				if(flag == 0) number = buff[bfOffBits+i];
				if((bfOffBits+i) == (512*3-1)) { bfOffBits=0; i=0; flag=1; break; }

				if(number == 0x00){
					number2 = buff[(flag==1) ? bfOffBits+i : bfOffBits+i+1];
					if(number2 == 0x00){ i = (flag==1) ? i+1 : i+2; bmx=0; bmy++; flag=0; }
					if(number2 == 0x01){ i = (flag==1) ? i+1 : i+2; flag=0; break;  }
					if(number2 == 0x02){
						if((bfOffBits+i+1) == (512*3-1)) { bfOffBits=0; i=0; flag=2; break; }

						for(int t=0; t<buff[(flag==1) ? bfOffBits+i+1 : bfOffBits+i+2]; t++){
							if(bmx > (biWidth-1)) {bmx=0; bmy++;}
							else bmx++;
						}
						bmy += buff[(flag==1) ? bfOffBits+i+2 : bfOffBits+i+3];
						i = (flag==1) ? i+3 : i+4; flag=0;
					}
					if(number2>=3 && number2<=255){

						for(j=0; j<number2; j++){
							if(bmx>biWidth-1) {bmx=0; bmy++;}
							int index = (flag==1) ? bfOffBits+i+1+j : bfOffBits+i+2+j;
							if(index == 512*3) {flag=3; break;}
							BSP_LCD_DrawPixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(buff[index], info, MODE));
							bmx++;
						}
						if(flag==3) { bfOffBits=0; i=0; break; }

						if(number2%2==0) i=i+number2+2; else i=i+number2+3;
						flag=0;
					}
				} else{
					number2 = buff[(flag==1) ? bfOffBits+i : bfOffBits+i+1];
					if((bfOffBits+i+1) == (512*3-1)) { bfOffBits=0; i=0; flag=4; break; }

					for(int c=0; c<number; c++){
						if(bmx>biWidth-1) {bmx=0;bmy++;}
						BSP_LCD_DrawPixel(bmx+ix, biHeight-1-bmy+iy, colorpalette(number2, info, MODE));
					    bmx++;
					}
					i = (flag==1) ? i+1 : i+2; flag=0;
				}
			}
		}
		if(p>t) break;
	}
	printf("\r--------------------DRAW--DONE--------------------------\r\n");
    while (1){}

}
