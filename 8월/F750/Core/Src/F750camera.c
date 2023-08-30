#include "main.h"
#include "stm32f7xx_hal.h"
#include "fonts.h"
#include "ff.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "stm32f7508_discovery.h"
#include "stm32f7508_discovery_lcd.h"
#include "stm32f7508_discovery_qspi.h"
#include "stm32f7508_discovery_camera.h"

extern UART_HandleTypeDef huart1;
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
unsigned short int *Camera = (unsigned short int *)0xC0040000;
#define putPixel(x,y,color) Buffer[x+y*480]=color
#define getCamera(x, y)		Camera[x+y*480]

int _write(int file, uint8_t *p, int len)
{	HAL_UART_Transmit(&huart1, p, len, 100);
   return len;
}

extern I2C_HandleTypeDef hi2c1;

void f750camera(void){
	HAL_TIM_Base_Start_IT(&htim2);
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

	uint8_t res;
	res = ov5640_ReadID(0x78);
	printf("READ 0x78 : %d\r\n", res);
	res = ov5640_ReadID(0x79);
	printf("READ 0x79 : %d\r\n", res);
	res = ov5640_ReadID(0x3c);
	printf("READ 0x3C : %d\r\n", res);
	res = ov5640_ReadID(0x3d);
	printf("READ 0x3D : %d\r\n", res);






	res = BSP_CAMERA_Init(CAMERA_R320x240);
	printf("CAMERA INIT : %d\r\n", res);




}
