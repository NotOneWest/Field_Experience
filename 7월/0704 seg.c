#include "main.h"
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

#define data_high() 	data_GPIO_Port->BSRR=data_Pin
#define data_low() 		data_GPIO_Port->BRR=data_Pin
#define shift_high() 	shift_GPIO_Port->BSRR=shift_Pin
#define shift_low() 	shift_GPIO_Port->BRR=shift_Pin
#define latch_high() 	latch_GPIO_Port->BSRR=latch_Pin
#define latch_low() 	latch_GPIO_Port->BRR=latch_Pin

unsigned char db[8] = {0x03, 0x9f, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f};
const unsigned char font[] = {0x03, 0x9f, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f, 0x01, 0x09}; // 0~9

static int sec;

void seg_off(void){
	unsigned int data=0x00ff;

	latch_low();
	for(int i=0; i<16; i++){
		shift_low();
		if(data & (1 << i)) { data_high();}	else{data_low();}
		shift_high();
	}
	latch_high();
}

void disp_seg(void){
	const unsigned int address[8] = {0x0800, 0x0400, 0x0200, 0x0100, 0x8000, 0x4000, 0x2000, 0x1000};
	static unsigned int data, j; // static 자동 초기화, 값 지우지 않고 계속 기억
	static int msec, min;
	if (msec<999) msec++;
	else
	{	msec=0;
		if (sec<58) sec++;
		else { sec=0; min++; }
	}

	j = (j<7) ? j+1 : 0;
	data = address[j] | db[j];

	latch_low();
	for(int i=0; i<16; i++){
		shift_low();
		if(data & (1 << i)) { data_high();}	else{data_low();}
		shift_high();
	}
	latch_high();
}

void dig4L(int k){
//	k = k % 10000;
	if(k < 10000){
		db[0] = font[ k /1000 ];
		db[1] = font[ ( k % 1000 )/100 ];
		db[2] = font[ ( k % 100 ) /10 ];
		db[3] = font[ k % 10 ];
	} else{ db[0] = 0xfd; db[1] = 0xfd; db[2] = 0xfd; db[3] = 0xfd;}
}

void dig4R(int k){
//	k = k % 10000;
	if(k < 10000){
		db[4] = font[ k /1000 ];
		db[5] = font[ ( k % 1000 )/100 ];
		db[6] = font[ ( k % 100 ) /10 ];
		db[7] = font[ k % 10 ];
	} else{ db[4] = 0xfd; db[5] = 0xfd; db[6] = 0xfd; db[7] = 0xfd;}
}
void dig8(unsigned int k){
	dig4L(k/10000);
	dig4R(k%10000);
}

void seg_main(void) {
	int i=0, old;
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

	while(1){
		i++;
		dig8(i);
		if (old != sec){
			old=sec;
			TIM2->CCR1 = 100 * (sec % 10);
			TIM1->CCR1 = 100 * (sec % 10);
		}
	}
}

