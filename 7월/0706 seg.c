#include "main.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;
extern ADC_HandleTypeDef hadc1;

#define data_high() 	data_GPIO_Port->BSRR=data_Pin
#define data_low() 		data_GPIO_Port->BRR=data_Pin
#define shift_high() 	shift_GPIO_Port->BSRR=shift_Pin
#define shift_low() 	shift_GPIO_Port->BRR=shift_Pin
#define latch_high() 	latch_GPIO_Port->BSRR=latch_Pin
#define latch_low() 	latch_GPIO_Port->BRR=latch_Pin

#define s0_high() 		s0_GPIO_Port->BSRR=s0_Pin
#define s0_low() 		s0_GPIO_Port->BRR=s0_Pin
#define s1_high() 		s1_GPIO_Port->BSRR=s1_Pin
#define s1_low() 		s1_GPIO_Port->BRR=s1_Pin
#define s2_high() 		s2_GPIO_Port->BSRR=s2_Pin
#define s2_low() 		s2_GPIO_Port->BRR=s2_Pin
#define s3_high() 		s3_GPIO_Port->BSRR=s3_Pin
#define s3_low() 		s3_GPIO_Port->BRR=s3_Pin

#define TRIG1_high() 	TRIG1_GPIO_Port->BSRR=TRIG1_Pin
#define TRIG1_low() 	TRIG1_GPIO_Port->BRR=TRIG1_Pin
#define TRIG2_high() 	TRIG2_GPIO_Port->BSRR=TRIG2_Pin
#define TRIG2_low() 	TRIG2_GPIO_Port->BRR=TRIG2_Pin
#define read_ECHO1() 	((ECHO1_GPIO_Port->IDR)&(ECHO1_Pin))
#define read_ECHO2() 	((ECHO2_GPIO_Port->IDR)&(ECHO2_Pin))
#define min(x, y) 		(x) < (y) ? (x) : (y)

unsigned char db[8] = {0x03, 0x9f, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f};
const unsigned int address[8] = {0x0800, 0x0400, 0x0200, 0x0100, 0x8000, 0x4000, 0x2000, 0x1000};
const unsigned char font[] = {0x03, 0x9f, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f, 0x01, 0x09}; // 0~9
static unsigned int sec, display_no, ADC_Val, position, y;

void step_move(void){
	if(y > position) position++;
	if(y < position) position--;

	switch(position%8) {
		case 0:
			s0_high(); s1_low(); s2_low(); s3_low(); break;
		case 1:
			s0_high(); s1_high(); s2_low(); s3_low(); break;
		case 2:
			s0_low(); s1_high(); s2_low(); s3_low(); break;
		case 3:
			s0_low(); s1_high(); s2_high(); s3_low(); break;
		case 4:
			s0_low(); s1_low(); s2_high(); s3_low(); break;
		case 5:
			s0_low(); s1_low(); s2_high(); s3_high(); break;
		case 6:
			s0_low(); s1_low(); s2_low(); s3_high(); break;
		case 7:
			s0_high(); s1_low(); s2_low(); s3_high(); break;
		default:
			break;
	}
}

void seg_off(void){
	if((sec%10) == display_no){
		latch_low();
		for(int i=0; i<16; i++){
			shift_low();
			if(0x00ff & (1 << i)) { data_high();}	else{data_low();}
			shift_high();
		}
		latch_high();
	} else if((sec%10) == 8){
		latch_low();
		for(int i=0; i<16; i++){
			shift_low();
			if(0x00ff & (1 << i)) { data_high();}	else{data_low();}
			shift_high();
		}
		latch_high();
	}
}

void disp_seg(void){
	static unsigned int data; // static 자동 초기화, 값 지우지 않고 계속 기억
	static int msec, min;
	if (msec<999) msec++;
	else
	{	msec=0;
		if (sec<59) sec++;
		else { sec=0; min++; }
	}

	display_no = (display_no<7) ? display_no+1 : 0;
	data = address[display_no] | db[display_no];

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
	int old, k=12;
	int mode=0, L1=4001, L2=4001, cnt1=0, cnt2=0;
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start(&htim6);

	while(1){
		switch(mode){
			case 0:
				TIM6->CNT = 0; TRIG1_high(); mode=1;
				break;
			case 1:
				if(TIM6->CNT >= 10) {TRIG1_low(); mode=2;}
				break;
			case 2:
				if(read_ECHO1()) {TIM6->CNT=0; mode=3;}
				break;
			case 3:
				if(read_ECHO1() == 0) {L1=min((TIM6->CNT * 17 / 100), L1); cnt1++; mode=4;}
				break;
			case 4:
				TIM6->CNT = 0; TRIG2_high(); mode=5;
				break;
			case 5:
				if(TIM6->CNT >= 10) {TRIG2_low(); mode=6;}
				break;
			case 6:
				if(read_ECHO2()) {TIM6->CNT=0; mode=7;}
				break;
			case 7:
				if(read_ECHO2() == 0) {L2=min((TIM6->CNT * 17 / 100), L2); cnt2++; mode=8;}
				break;
			default:
				break;
		}
		if(TIM6->CNT > 30000) mode = (mode<4 || mode==8) ? 0 : 4;
		if(cnt1==10){ dig4L(L1); cnt1=0; L1=4001; }
		if(cnt2==10){ dig4R(L2); cnt2=0; L2=4001; }


//		HAL_ADC_Start(&hadc1);
//		ADC_Val=HAL_ADC_GetValue(&hadc1);
//		dig4L(ADC_Val);
//		y = (k*y + ADC_Val) / (k+1);
//		dig4L(y);
//		dig4R(position);
		if (old != sec){
			old=sec;
			TIM2->CCR1 = 100 * (sec % 10);
			TIM1->CCR1 = 500;
		}
	}
}

