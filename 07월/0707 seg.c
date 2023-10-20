#include "main.h"
#include "math.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;
extern ADC_HandleTypeDef hadc1;
extern DAC_HandleTypeDef hdac1;

void  	data_high(void) {  data_GPIO_Port->BSRR=data_Pin; }
#define data_low() 		data_GPIO_Port->BRR=data_Pin
#define read_data()     ((data_GPIO_Port->IDR)&(data_Pin))
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

#define PI       		3.14159265358979323846

//unsigned char db[8] = {0x03, 0x9f, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f};
const unsigned int address[8] = {0x0800, 0x0400, 0x0200, 0x0100, 0x8000, 0x4000, 0x2000, 0x1000};
//const unsigned char font[] = {0x03, 0x9f, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f, 0x01, 0x09}; // 0~9
static unsigned int sec, display_no, ADC_Val, position, y;

unsigned char db[16]={0x7f,0x00,0x6f,0x00,0x77,0x00,0x7c,0x00,0x5e,0x00,0x79,0x00,0x71,0x00,0x039,0x00};
const unsigned char font[17] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71}; // 0~9, A, b, C, d, E, F
unsigned int key[4], sin_data[100];

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

void dig_key8(int bit_1, int bit_2, int bit_3, int bit_4) {
	db[0]= font[bit_1 >> 4];
	db[2]= font[bit_1 & 0x0f];
	db[4]= font[bit_2 >> 4];
	db[6]= font[bit_2 & 0x0f];
	db[8]= font[bit_3 >> 4];
	db[10]= font[bit_3 & 0x0f];
	db[12]= font[bit_4 >> 4];
	db[14]= font[bit_4 & 0x0f];
}

void seg8(unsigned char data){
	unsigned int j;
	for(j=0; j<8; j++){
		shift_low();
		if(data & (1 << j)) {data_high();} else{data_low();}
		shift_high();
	}
}

void seg_comm(unsigned char data){
	latch_low();
	seg8(data);
	latch_high();
}

void seg_disp(){
	int j;
	seg_comm(0x48);	//Wr, Auto Inc
	latch_low();
	seg8(0xC0);
	for (j=0;j<16;j++) seg8(db[j]);
	latch_high();
}

void read(){
	latch_low();
	seg8(0x42);
	data_high();
	unsigned int i, j, k;
	for(j=0; j<4; j++){
		k=0;
		for(i=0; i<8; i++){
			shift_low();
			k = k << 1;
			shift_high();
			if(read_data()) k++;
		}
		key[j]=k;
	}
	latch_high();
}

void keyboard(){
	if(key[0]==128) TIM2->ARR = 1223;
	else if(key[1]==128) TIM2->ARR = 1154;
	else if(key[2]==128) TIM2->ARR = 1089;
	else if(key[3]==128) TIM2->ARR = 1028;
	else if(key[0]==8) TIM2->ARR = 970;
	else if(key[1]==8) TIM2->ARR = 916;
	else if(key[2]==8) TIM2->ARR = 864;
	else if(key[3]==8) TIM2->ARR = 816;
}

void sound(void){
	static int i;

	i = (i<99) ? i+1 : 0;
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, sin_data[i]);
}

void seg_main(void) {
	int old, k=12;
	int mode=0, L1=4001, L2=4001, cnt1=0, cnt2=0;
	seg_comm(0x8F);	// Duty 14/16,  Disp On

	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
//	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);

	for(int i=0; i<100; i++) sin_data[i] = 1000 * (1 + sin((PI*i)/50));

	while(1){
		seg_disp();
		dig_key8(key[0], key[1], key[2], key[3]);
		read();
		keyboard();

//		switch(mode){
//			case 0:
//				TIM6->CNT = 0; TRIG1_high(); mode=1;
//				break;
//			case 1:
//				if(TIM6->CNT >= 10) {TRIG1_low(); mode=2;}
//				break;
//			case 2:
//				if(read_ECHO1()) {TIM6->CNT=0; mode=3;}
//				break;
//			case 3:
//				if(read_ECHO1() == 0) {L1=min((TIM6->CNT * 17 / 100), L1); cnt1++; mode=4;}
//				break;
//			case 4:
//				TIM6->CNT = 0; TRIG2_high(); mode=5;
//				break;
//			case 5:
//				if(TIM6->CNT >= 10) {TRIG2_low(); mode=6;}
//				break;
//			case 6:
//				if(read_ECHO2()) {TIM6->CNT=0; mode=7;}
//				break;
//			case 7:
//				if(read_ECHO2() == 0) {L2=min((TIM6->CNT * 17 / 100), L2); cnt2++; mode=8;}
//				break;
//			default:
//				break;
//		}
//		if(TIM6->CNT > 30000) mode = (mode<4 || mode==8) ? 0 : 4;
//		if(cnt1==10){ dig4L(L1); mode=0; cnt1=0; L1=4001; }
//		if(cnt2==10){ dig4R(L2); mode=0; cnt2=0; L2=4001; }

//		HAL_ADC_Start(&hadc1);
//		ADC_Val=HAL_ADC_GetValue(&hadc1);
//		dig4L(ADC_Val);
//		y = (k*y + ADC_Val) / (k+1);
//		dig4L(y);
//		dig4R(position);
//		if (old != sec){
//			old=sec;
//			TIM2->CCR1 = 100 * (sec % 10);
//			TIM1->CCR1 = 500;
//		}
	}
}

