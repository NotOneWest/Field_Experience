//#include "main.h"
//#include "stm32f7xx_hal.h"
//#include <stdlib.h>
//#include "stm32f7508_discovery.h"
//#include "stm32f7508_discovery_lcd.h"
//#include <stdio.h>
//#include "stm32f7508_discovery_qspi.h"
//#include "fonts.h"
//#include <math.h>
//extern UART_HandleTypeDef huart1;
//extern ADC_HandleTypeDef hadc3;
//extern TIM_HandleTypeDef htim3;
//
//#define  MAX_Y       271
//#define  BLACK       0x0000      /*   0,   0,   0 */
//#define  NAVY        0x000F      /*   0,   0, 128 */
//#define  DARKGREEN   0x03E0      /*   0, 128,   0 */
//#define  DARKCYAN    0x03EF      /*   0, 128, 128 */
//#define  MAROON      0x7800      /* 128,   0,   0 */
//#define  PURPLE      0x780F      /* 128,   0, 128 */
//#define  OLIVE       0x7BE0      /* 128, 128,   0 */
//#define  LIGHTGREY   0xC618      /* 192, 192, 192 */
//#define  DARKGREY    0x7BEF      /* 128, 128, 128 */
//#define  BLUE        0x001F      /*   0,   0, 255 */
//#define  GREEN       0x07E0      /*   0, 255,   0 */
//#define  CYAN        0x07FF      /*   0, 255, 255 */
//#define  RED         0xF800      /* 255,   0,   0 */
//#define  MAGENTA     0xF81F      /* 255,   0, 255 */
//#define  YELLOW      0xFFE0      /* 255, 255,   0 */
//#define  WHITE       0xFFFF      /* 255, 255, 255 */
//#define  ORANGE      0xFD20      /* 255, 165,   0 */
//#define  GREENYELLOW 0xAFE5      /* 173, 255,  47 */
//const unsigned short int  COLOR_TABLE[16] = {  BLACK, BLUE, GREEN, RED, NAVY, DARKGREEN, DARKCYAN,
//						MAROON, PURPLE, OLIVE, LIGHTGREY, DARKGREY, CYAN, MAGENTA, ORANGE, GREENYELLOW};
//void testdrawline() {
//    BSP_LCD_Clear( BLACK);
//
//    for (int16_t i=0; i<480; i+=8) {
//        BSP_LCD_DrawLine(0, 0, i, 272-1);
//        HAL_Delay(10);
//    }
//    for (int16_t i=0; i<272; i+=8) {
//        BSP_LCD_DrawLine(0, 0, 480-1, i);
//        HAL_Delay(10);
//    }
//    HAL_Delay(100);
//
//    BSP_LCD_Clear( BLACK);
//    for (int16_t i=0; i<480; i+=8) {
//        BSP_LCD_DrawLine(0, 272-1, i, 0);
//        HAL_Delay(10);
//    }
//    for (int16_t i=272-1; i>=0; i-=8) {
//        BSP_LCD_DrawLine(0, 272-1, 480-1, i);
//        HAL_Delay(10);
//    }
//    HAL_Delay(100);
//}
//unsigned short int *Buffer = (unsigned short int *)0xC0000000;
////void putPixel(int x, int y, int color) { Buffer[x+y*480] = color; }
//#define putPixel(x,y,color) Buffer[x+y*480]=color
//
//int _write(int file, uint8_t *p, int len)
//{	HAL_UART_Transmit(&huart1, p, len, 100);
//   return len;
//}
//
//int x=0, adv=0, adv_no=0, filter, diff, notch_no=0, idx=2;
//long HR, prev_RR=0, N=0, sum=0, AVE=0, square=0, SDNN=0, dif=0, RMSSD=0, condition=0;
//int HR_MAX=200, HR_MIN=50, T_MAX=1200, T_MIN=300, cut=125, MAX=0;
//int adv_val[16], notch[20], ADV[2]={0, }, FILTER[2]={0, }, DIFF[2]={0, };
//
////int abs(int num) {if(num < 0) num=(-num); return num;}
//
//void msecint(){
//	static int graph_count, timer_count, flag_count, adv_max, adv_min, filter_max, filter_min, diff_max, diff_min;
//	HAL_ADC_Start(&hadc3);
//	adv = filter = HAL_ADC_GetValue(&hadc3);
//
//	for(int i=0; i<16; i++) filter+=adv_val[i];
//	adv_no = (adv_no<15) ? adv_no+1 : 0;
//	adv_val[adv_no]=adv;
//
//	diff=abs(notch[notch_no] - filter);
//	notch[notch_no]=filter;
//	notch_no = (notch_no<19) ? notch_no+1 : 0;
//
//	if(MAX < (diff/200) && ((diff/200) > cut)) { MAX=(diff/200); flag_count=0; }
//	else{
//		if(flag_count > T_MIN){
//			prev_RR = HR;
//			HR = (timer_count);
//			timer_count=0; flag_count=0;
//			cut = (MAX*3)/4; MAX=0; idx  = (idx+4)%8;
//
//			N++;
//			sum += HR; square += (HR*HR);
//			AVE = sum/N; SDNN = sqrt(square/N-(AVE*AVE));
//			if(N > 1) { dif += ((HR - prev_RR)*(HR - prev_RR)); RMSSD = sqrt(dif/(N-1)); }
//			condition=(SDNN + RMSSD);
//		}
//	}
//	if(MAX < cut) flag_count=0;
//	if(timer_count > T_MAX) { timer_count=0; flag_count=0; MAX=0; cut=0; }
//	timer_count++; flag_count++;
//
//	if(adv_max < adv) adv_max=adv;
//	if(adv_min > adv) adv_min=adv;
//
//	if(filter_max < filter) filter_max=filter;
//	if(filter_min > filter) filter_min=filter;
//
//	if(diff_max < diff) diff_max=diff;
//	if(diff_min > diff) diff_min=diff;
//
//	graph_count++;
//	if(graph_count >= 15){
//		ADV[0] = adv_max; ADV[1] = adv_min;
//		FILTER[0] = filter_max; FILTER[1] = filter_min;
//		DIFF[0] = diff_max; DIFF[1] = diff_min;
//
//		adv_max=adv_min=adv; filter_max=filter_min=filter; diff_max=diff_min=diff;
//		graph_count=0;
//	}
//}
//
//uint8_t buff[1000];
//void f750bsp(void)
//{
//	HAL_TIM_Base_Start_IT(&htim3);
////	int color=0, y;
//	int x=0, i=0;
//	BSP_LED_Init(0);
//	  BSP_LCD_Init();
//	  BSP_LCD_Init();
//	  BSP_LCD_LayerRgb565Init(0, LCD_FB_START_ADDRESS);
//	  //BSP_LCD_LayerRgb565Init(1, LCD_FB_START_ADDRESS+(BSP_LCD_GetXSize()*BSP_LCD_GetYSize()*2)); // 480 * 272 * 2 = 261,120
//	  BSP_LCD_DisplayOn();
//	  BSP_LCD_SelectLayer(0);
//	  BSP_LCD_Clear(BLACK);
////	  BSP_LCD_SelectLayer(1);
////	  BSP_LCD_Clear( BLACK);
////	  BSP_LCD_SetTransparency(0, 255);
////	  BSP_LCD_SetTransparency(1, 127);
//	  BSP_LCD_SelectLayer(0);
//	  BSP_LCD_SetTextColor(GREEN);
//	  i=BSP_QSPI_Init ();
//	  printf("\n\rBSP_QSPI_Init = %d\n\r",i);
//	  i= BSP_QSPI_EnableMemoryMappedMode();
//	  printf("\n\rEnableMemoryMapped = %d\n\r",i);
//	  i= BSP_QSPI_Read(buff,0,100);
//	  printf("QSPI_Read = %d\n\r",i);
//	  for (i=28;i<80;i++)
//	  {  if ((i%10)==0) printf("\n\r");
//	  	  printf("%2X ",buff[i]);
//	  }
//	  printf("\n\r");
//
//	  i=0;
//	  while (1){
//
//		  HAL_Delay(1);
//		  if(ADV[1]){
//			  x = (x<480) ? x+1 : 0;
//			  BSP_LCD_SetTextColor(BLACK);
//			  BSP_LCD_DrawVLine(x, 40, 272);
//			  BSP_LCD_SetTextColor(YELLOW);
//			  BSP_LCD_DrawLine(x, (FILTER[1]/200), x, (FILTER[0]/200));
//			  BSP_LCD_SetTextColor(RED);
//			  BSP_LCD_DrawLine(x, MAX_Y-(DIFF[1]/200), x, MAX_Y-(DIFF[0]/200));
////			  BSP_LCD_SetTextColor(GREEN);
////			  BSP_LCD_DrawLine(x, (ADV[1]*17-11000)/200, x, (ADV[0]*17-11000)/200);
//			  BSP_LCD_DrawPixel(x, (MAX_Y-MAX), MAGENTA);
//
//			  BSP_LCD_SetBackColor(BLACK);
//			  BSP_LCD_SetTextColor(GREEN);
//			  BSP_LCD_SetFont(&Font16);
//
//			  char* show = "HR: ";
//			  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*) show, LEFT_MODE);
//			  BSP_LCD_DisplayChar(66, 0,(60000/HR)/100+'0');
//			  BSP_LCD_DisplayChar(77, 0,((60000/HR)/10)%10+'0');
//			  BSP_LCD_DisplayChar(88, 0,(60000/HR)%10+'0');
//
//			  show = " AVE: ";
//			  BSP_LCD_DisplayStringAt(154, 0, (uint8_t*) show, LEFT_MODE);
//			  BSP_LCD_DisplayChar(232, 0,(60000/AVE)/100+'0');
//			  BSP_LCD_DisplayChar(242, 0,((60000/AVE)/10)%10+'0');
//			  BSP_LCD_DisplayChar(253, 0,(60000/AVE)%10+'0');
//
//			  show = "SDNN: ";
//			  BSP_LCD_DisplayStringAt(0, 16, (uint8_t*) show, LEFT_MODE);
//			  BSP_LCD_DisplayChar(66, 16,SDNN/100+'0');
//			  BSP_LCD_DisplayChar(77, 16,(SDNN/10)%10+'0');
//			  BSP_LCD_DisplayChar(88, 16,SDNN%10+'0');
//
//			  show = " RMSSD: ";
//			  BSP_LCD_DisplayStringAt(154, 16, (uint8_t*) show, LEFT_MODE);
//			  BSP_LCD_DisplayChar(231, 16,RMSSD/100+'0');
//			  BSP_LCD_DisplayChar(242, 16,(RMSSD/10)%10+'0');
//			  BSP_LCD_DisplayChar(253, 16,RMSSD%10+'0');
//
//			  show = " COND: ";
//			  BSP_LCD_DisplayStringAt(308, 16, (uint8_t*) show, LEFT_MODE);
//			  BSP_LCD_DisplayChar(385, 16,condition/100+'0');
//			  BSP_LCD_DisplayChar(396, 16,(condition/10)%10+'0');
//			  BSP_LCD_DisplayChar(407, 16,condition%10+'0');
//
//			  ADV[1]=0;
//		  }
//	  }
//}
