#include "main.h"
#include <stdio.h>
#include <stdlib.h>

extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;


extern ADC_HandleTypeDef hadc3;
//extern COMP_HandleTypeDef hcomp1;
extern DAC_HandleTypeDef hdac1;

int _write(int file, uint8_t *p, int len){
	HAL_UART_Transmit(&huart1, p, len, 100);
	return len;
}

unsigned char received[2];

uint32_t v = 0;
uint16_t ADC_buf[200];

#define Kp 0.58
#define Ki 0.035
// 10ms 0.41 0.0083

int ADC=0, flag=0;
int MIN=0, MAX=0, AVG=0, DELTA=0;
int err, perv_err, Op, Oi=0, Control;

void pwmUpdate(){ flag=1; }

void pControl(){
	static int min_cnt=0;
	for(int i=0; i<100; i++){
		if(ADC_buf[i] != 0) min_cnt++;
		if(min_cnt == 4) MIN = ADC_buf[i];
		if(MAX < ADC_buf[i]) MAX = ADC_buf[i];
	}
	AVG = (MAX+MIN)/2;

	err = (v - AVG);
	Op = (Kp * err);
	Oi += (Ki * err);

	Control = (Op + Oi);

	if(Control > 1000) Control=1000;
	else if(Control < 0) Control=0;
	TIM1->CCR1 = Control;

	HAL_DAC_SetValue(&hdac1, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, Control);

	MIN=MAX=min_cnt=0;
}

void intensity(){
//	static int cnt=0, min_cnt=0;
//	HAL_ADC_Start(&hadc3);
//	ADC[cnt] = HAL_ADC_GetValue(&hadc3);
//
//	if(min_cnt > 0){
//		if(ADC[cnt] == 0){ AVG = (MIN+MAX)/2; DELTA = (MAX-AVG); MIN=MAX=min_cnt=0; flag=2; }
//
//		min_cnt++;
//		if(min_cnt == 3) MIN = ADC[cnt];
//	}
//	if(flag == 1){ min_cnt++; flag=0; }
//	if(min_cnt != 0){
//		if(MAX < ADC[cnt]) MAX = ADC[cnt];
//	}
//	cnt++;
//	HAL_ADC_Stop(&hadc3);
}



void ccc(void){
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
	HAL_TIM_Base_Start(&htim2);

	HAL_ADC_Start_DMA(&hadc3, (uint32_t *)ADC_buf, 200);

//	HAL_COMP_Start(&hcomp1);

	nSleep_GPIO_Port -> BSRR = nSleep_Pin;		// high
	Disable_GPIO_Port-> BRR  = Disable_Pin;		// low

	TIM1->CCR1 = 10;

	HAL_DAC_Start(&hdac1, DAC1_CHANNEL_1);

	int i=0;
	while(1){
		v = ((i%21) * 4095) / 33;
		HAL_Delay(1000);
		i+=5; HAL_Delay(500);
//		if(HAL_UART_Receive_IT(&huart1, received, 2) == HAL_OK){
//			if(received[0] >= '0' && received[0] <= '9' && received[1] >= '0' && received[1] <= '9'){
//				v = (((received[0] - '0')*10 + (received[1] - '0')) * 4095) / 33;
//				printf("INPUT = %c.%c / ", received[0], received[1]); printf("V : %lu \r\n", v);
//			}
//		}


//		if(flag=1){
//			printf("\r\n_______________________________\r\n");
//			for(int i=0; i<100; i++) printf("ADV : %d\r\n" ,ADC_buf[i]);
//			printf("_____________AVG : %d__________________\r\n", AVG);
//		}
//		if(DELTA != 0) printf("DELTA : %d \r\n", DELTA);
		printf("[V = %lu] ERR : %d / Control : %d / DUTY : %lu (AVG = %d)\r\n", v, err, Control, TIM1->CCR1, AVG);

	}
}
