//#include "main.h"
//#include "stm32f7xx_hal.h"
//#include <stdlib.h>
//#include "stm32f7508_discovery.h"
//#include "stm32f7508_discovery_lcd.h"
//#include <stdio.h>
//#include "stm32f7508_discovery_qspi.h"
//#include "fonts.h"
//#include "ff.h"
//#include "stm32f7508_discovery_audio.h"
//#include <math.h>
//#include <string.h>
//extern UART_HandleTypeDef huart1;
//extern TIM_HandleTypeDef htim2;
//
//#define  MAX_Y       271
//
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
//
//unsigned short int *Buffer = (unsigned short int *)0xC0000000;
//#define putPixel(x,y,color) Buffer[x+y*480]=color
//
//int _write(int file, uint8_t *p, int len)
//{	HAL_UART_Transmit(&huart1, p, len, 100);
//   return len;
//}
//
//FATFS 	SDFatFs; /* File system object for SD card logical drive */
//FIL 	MyFile; /* File object */
//FRESULT res;
//UINT 	bytesread;
//
//#define AUDIO_BUFFER_SIZE 4096
//#define max(a,b)  (((a) < (b)) ? (b) : (a))
//#define min(a,b)  (((a) < (b)) ? (a) : (b))
//#define PI 3.14159265358979
//
//int fft(uint32_t N, double XR[], double XI[]){
//    /* 1. check 2^n count -> if else then return -1 */
//    if ((N !=0) && ((N & (N-1)) != 0) ) return -1;
//
//    /* 2. calculate W */
//    long N2 = N >> 1;
//    double WR[N2], WI[N2]; /* Real and Imaginary part */
//    double T = 2 * PI / N;
//    for(long i=0;i<N2;++i){
//        WR[i] = cos(T*i);
//        WI[i] = -sin(T*i);
//    }
//
//    /* 3. shuffle input array index to calculate bottom-up style FFT */
//    int log2N = (int)log2(N);
//    double tmp;
//    for(long n=1;n<N-1;++n){
//        long m = 0; /* reversed num of n*/
//        for(int i=0;i<log2N;++i){
//            m |= ((n >> i) & 1) << (log2N-i-1); /* reverse the bits */
//        }
//        if(n<m){ /* exchange */
//            tmp = XR[n];
//            XR[n] = XR[m];
//            XR[m] = tmp;
//        }
//    }
//
//    for(int i=0; i<N; i++) XI[i]=0;
//
//    /* 4. execute fft */
//    for(int loop=0; loop<log2N; ++loop){
//        long regionSize = 1 << (loop+1);      /* if N=8: 2 -> 4 -> 8 */
//        long kJump = 1 << (log2N - loop - 1); /* if N=8: 4 -> 2 -> 1 */
//        long half = regionSize >> 1;
//        for(register long i=0; i<N; i += regionSize){
//            long blockEnd = i + half -1;
//            long k=0;
//            double TR, TI;
//            for(register long j=i; j<=blockEnd; ++j){ /* j start from i */
//                TR = WR[k] * XR[j+half] - WI[k] * XI[j+half];
//                TI = WI[k] * XR[j+half] + WR[k] * XI[j+half];
//
//                XR[j+half] = XR[j] - TR;
//                XI[j+half] = XI[j] - TI;
//
//                XR[j] = XR[j] + TR;
//                XI[j] = XI[j] + TI;
//                k += kJump;
//            }
//        }
//    }
//    return 0;
//}
//
//typedef enum{
//  BUFFER_OFFSET_NONE = 0,
//  BUFFER_OFFSET_HALF,
//  BUFFER_OFFSET_FULL,
//} BUFFER_StateTypeDef;
//
//typedef struct{
//  uint32_t   ChunkID;       /* 0 */
//  uint32_t   FileSize;      /* 4 */
//  uint32_t   FileFormat;    /* 8 */
//  uint32_t   SubChunk1ID;   /* 12 */
//  uint32_t   SubChunk1Size; /* 16*/
//  uint16_t   AudioFormat;   /* 20 */
//  uint16_t   NbrChannels;   /* 22 */
//  uint32_t   SampleRate;    /* 24 */
//
//  uint32_t   ByteRate;      /* 28 */
//  uint16_t   BlockAlign;    /* 32 */
//  uint16_t   BitPerSample;  /* 34 */
//  uint32_t   SubChunk2ID;   /* 36 */
//  uint32_t   SubChunk2Size; /* 40 */
//} WAVE_FormatTypeDef;
//
///* Audio wave data length to be played */
//static uint32_t WaveDataLength = 0;
///* Audio wave remaining data length to be played */
//static uint32_t AudioRemSize = 0;
//int AudioPlayStart = 0;
//uint8_t buff[AUDIO_BUFFER_SIZE], buff2[AUDIO_BUFFER_SIZE];
//double XR[AUDIO_BUFFER_SIZE], XI[AUDIO_BUFFER_SIZE];
//static uint32_t fptr = 0;
//
//int min=0, sec=0, msec=0;
//int Xpos=0, flag=0, flag2=0;
//int prev_sum=0, sum=0, cnt=0;
//
//void readWAV(){
//	static int x=0;
//	if(AudioPlayStart){
//		for(int i=0; i<45; i++){
//			if(sec==60) {min++; sec=0;}
//			if(msec==1000) {sec++; msec=0;}
//			msec++;
//		}
//
//		bytesread = 0;
//		f_lseek(&MyFile, fptr*AUDIO_BUFFER_SIZE);
//		if(AudioRemSize >= AUDIO_BUFFER_SIZE) {
//			f_read(&MyFile, buff, AUDIO_BUFFER_SIZE, (void *)&bytesread);
//			if(bytesread > 0){ fptr++; AudioRemSize -= bytesread; }
//		} else{
//			f_read(&MyFile, buff, AudioRemSize, (void *)&bytesread);
//			if(bytesread > 0){ fptr++; AudioRemSize = 0; }
//		}
//
//		if(!flag){
//			for(int i=0; i<min(AUDIO_BUFFER_SIZE, AudioRemSize); i++) XR[i] = buff[i];
//			flag=1;
//		}
//
//		for(int i=0; i<min(AUDIO_BUFFER_SIZE, AudioRemSize); i++){
//			sum += buff[i]; cnt++;
//			if(cnt==60){
//				BSP_LCD_SetTextColor(BLACK);
//				BSP_LCD_DrawVLine(x, 0, 100);
//
//				BSP_LCD_SetTextColor(GREEN);
//				BSP_LCD_DrawLine(x, prev_sum%100, x, (sum/60)%100);
//				prev_sum=sum/60; sum=0; cnt=0;
//				x = (x<479) ? x+1 : 0;
//			}
//		}
//
//		if(AudioRemSize == 0) AudioPlayStart=0;
//	}
//}
//
//void f750wav(void){
//	HAL_TIM_Base_Start_IT(&htim2);
//	int x=0;
//	BSP_LED_Init(0);
//	BSP_LCD_Init();
//	BSP_LCD_Init();
//	BSP_LCD_LayerRgb565Init(0, LCD_FB_START_ADDRESS);
//	BSP_LCD_DisplayOn();
//    BSP_LCD_SelectLayer(0);
//    BSP_LCD_Clear(BLACK);
//    x=BSP_QSPI_Init ();
//	printf("\n\rBSP_QSPI_Init = %d\n\r",x);
//	x= BSP_QSPI_EnableMemoryMappedMode();
//	printf("\n\rEnableMemoryMapped = %d\n\r",x);
//
//    res = f_mount(&SDFatFs, "", 0);
//    if(res != 0) printf("MOUNT ERROR : %d\r\n", res);
//
//    while(1){
//    	res = f_open(&MyFile, "audio_sample.wav", FA_READ);
//    	if(res != 0) {printf("OPEN ERROR : %d\r\n", res);}
//    	else break;
//    }
//
//    WAVE_FormatTypeDef waveformat;
//    f_read (&MyFile, &waveformat, sizeof(waveformat), &bytesread);
//    AudioRemSize = WaveDataLength = waveformat.FileSize;
//
//	x=BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, 25, waveformat.SampleRate);
//    printf("\n\rBSP_AUDIO_OUT_Init 1 = %d\n\r",x);
//
//    AudioPlayStart = 1;
//    while(1){
//
//    	if(flag){
//    		int FFT=0;
//    		fft(AUDIO_BUFFER_SIZE , XR, XI);
//			for(int i=0; i<min(AUDIO_BUFFER_SIZE, AudioRemSize); i++){
//				FFT = sqrt(XR[i]*XR[i]+XI[i]*XI[i]);
//
//				if(i%128==flag2){
//					BSP_LCD_SetTextColor(BLACK);
//					BSP_LCD_FillRect(Xpos, 102, 15, 168);
//					BSP_LCD_SetTextColor(GREENYELLOW);
//					BSP_LCD_FillRect(0, 262, 480, 5);
//					for(int j=32; j>0; j--){
//						if(128*(j-1) < FFT && FFT <= 128*j){
//							BSP_LCD_SetTextColor(GREENYELLOW);
//							BSP_LCD_FillRect(Xpos, 104+(5*(32-j)), 15, 5*j);
//						}
//					}
//					Xpos = (Xpos<478) ? Xpos+15 : 0;
//				}
//			}
//    		flag=0; flag2 = (flag2<127) ? flag+1 : 0;
//		}
//
//    	BSP_AUDIO_OUT_Play((uint16_t*)buff, AUDIO_BUFFER_SIZE);
//
//    	if(!AudioPlayStart){
//			printf("\r\nTIME : %d min/ %d sec/ %d msec\r\n", min, sec, msec);
//			BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW);
//			/* Close file */
//			f_close(&MyFile);
//			break;
//		}
//    }
//}
