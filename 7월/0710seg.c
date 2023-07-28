#include "main.h"
#include "math.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern DAC_HandleTypeDef hdac1;

#define data_high() 	data_GPIO_Port->BSRR=data_Pin
#define data_low() 		data_GPIO_Port->BRR=data_Pin
#define read_data()     ((data_GPIO_Port->IDR)&(data_Pin))
#define shift_high() 	shift_GPIO_Port->BSRR=shift_Pin
#define shift_low() 	shift_GPIO_Port->BRR=shift_Pin
#define latch_high() 	latch_GPIO_Port->BSRR=latch_Pin
#define latch_low() 	latch_GPIO_Port->BRR=latch_Pin

#define PI       		3.14159265358979323846

unsigned char db[32]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char font[17] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; // 0~8번
unsigned int key[4], sin_data[100], sin2=0;

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

int checkPiano(){
	for(int i=0; i<4; i++){
		if(key[i]!=128) continue;
		for(int j=(i+1); j<8; j++){
			if(j<4){
				if(key[j]==128)  return 1;
			} else{
				if(key[j-4]==8) return 1;
			}
		}
	}
	for(int i=0; i<3; i++){
		if(key[i]!=8) continue;
		for(int j=(i+1); j<4; j++){
			if(key[j]==8) return 1;
		}
	}
	return 0;
}

void keyboard(){
	if(key[0]==128) {
		TIM2->ARR = 1223;
		if(key[1]==128) TIM3->ARR = 1224;
		else if(key[2]==128) TIM3->ARR = 1089;
		else if(key[3]==128) TIM3->ARR = 1028;
		else if(key[0]==8) TIM3->ARR = 970;
		else if(key[1]==8) TIM3->ARR = 916;
		else if(key[2]==8) TIM3->ARR = 864;
		else if(key[3]==8) TIM3->ARR = 816;
		else TIM3->ARR = 0;
	} else if(key[1]==128) {
		TIM2->ARR = 1154;
		if(key[0]==128) TIM3->ARR = 1223;
		else if(key[2]==128) TIM3->ARR = 1089;
		else if(key[3]==128) TIM3->ARR = 1028;
		else if(key[0]==8) TIM3->ARR = 970;
		else if(key[1]==8) TIM3->ARR = 916;
		else if(key[2]==8) TIM3->ARR = 864;
		else if(key[3]==8) TIM3->ARR = 816;
		else TIM3->ARR = 0;
	} else if(key[2]==128) {
		TIM2->ARR = 1089;
		if(key[0]==128) TIM3->ARR = 1223;
		else if(key[1]==128) TIM3->ARR = 1154;
		else if(key[3]==128) TIM3->ARR = 1028;
		else if(key[0]==8) TIM3->ARR = 970;
		else if(key[1]==8) TIM3->ARR = 916;
		else if(key[2]==8) TIM3->ARR = 864;
		else if(key[3]==8) TIM3->ARR = 816;
		else TIM3->ARR = 0;
	} else if(key[3]==128) {
		TIM2->ARR = 1028;
		if(key[0]==128) TIM3->ARR = 1223;
		else if(key[1]==128) TIM3->ARR = 1154;
		else if(key[2]==128) TIM3->ARR = 1089;
		else if(key[0]==8) TIM3->ARR = 970;
		else if(key[1]==8) TIM3->ARR = 916;
		else if(key[2]==8) TIM3->ARR = 864;
		else if(key[3]==8) TIM3->ARR = 816;
		else TIM3->ARR = 0;
	} else if(key[0]==8) {
		TIM2->ARR = 970;
		if(key[0]==128) TIM3->ARR = 1223;
		else if(key[1]==128) TIM3->ARR = 1154;
		else if(key[2]==128) TIM3->ARR = 1089;
		else if(key[3]==128) TIM3->ARR = 1028;
		else if(key[1]==8) TIM3->ARR = 916;
		else if(key[2]==8) TIM3->ARR = 864;
		else if(key[3]==8) TIM3->ARR = 816;
		else TIM3->ARR = 0;
	} else if(key[1]==8) {
		TIM2->ARR = 916;
		if(key[0]==128) TIM3->ARR = 1223;
		else if(key[1]==128) TIM3->ARR = 1154;
		else if(key[2]==128) TIM3->ARR = 1089;
		else if(key[3]==128) TIM3->ARR = 1028;
		else if(key[0]==8) TIM3->ARR = 970;
		else if(key[2]==8) TIM3->ARR = 864;
		else if(key[3]==8) TIM3->ARR = 816;
		else TIM3->ARR = 0;
	} else if(key[2]==8) {
		TIM2->ARR = 864;
		if(key[0]==128) TIM3->ARR = 1223;
		else if(key[1]==128) TIM3->ARR = 1154;
		else if(key[2]==128) TIM3->ARR = 1089;
		else if(key[3]==128) TIM3->ARR = 1028;
		else if(key[0]==8) TIM3->ARR = 970;
		else if(key[1]==8) TIM3->ARR = 916;
		else if(key[3]==8) TIM3->ARR = 816;
		else TIM3->ARR = 0;
	}
	else if(key[3]==8) {
		TIM2->ARR = 816;
		if(key[0]==128) TIM3->ARR = 1223;
		else if(key[1]==128) TIM3->ARR = 1154;
		else if(key[2]==128) TIM3->ARR = 1089;
		else if(key[3]==128) TIM3->ARR = 1028;
		else if(key[0]==8) TIM3->ARR = 970;
		else if(key[1]==8) TIM3->ARR = 916;
		else if(key[2]==8) TIM3->ARR = 864;
		else TIM3->ARR = 0;

	} else TIM2->ARR =0;
}

void sound(void){
	static int i;

	if(!checkPiano()) sin2=0;

	i = (i<99) ? i+1 : 0;
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, sin2 + sin_data[i]);
}

void sound2(void){
	static int i;

	i = (i<99) ? i+1 : 0;
	sin2 = sin_data[i];
}

void seg16(unsigned int data){
	unsigned int j;
	for(j=0; j<16; j++){
		shift_low();
		if(data & (1 << (15-j))) {data_high();} else{data_low();}
		shift_high();
	}
}

void init_dot(void){
	latch_low();
	for(int i=0; i<4; i++) seg16(0x0900);
	latch_high();

	latch_low();
	for(int i=0; i<4; i++) seg16(0x0a01);
	latch_high();

	latch_low();
	for(int i=0; i<4; i++) seg16(0x0b07);
	latch_high();

	latch_low();
	for(int i=0; i<4; i++) seg16(0x0c01);
	latch_high();

	latch_low();
	for(int i=0; i<4; i++) seg16(0x0f00);
	latch_high();
}

void db_on() {
	unsigned int data;
	for(int y=0; y<8; y++){
		latch_low();
		for(int x=0; x<4; x++){
			data = ((y+1) << 8) | db[4*y + x];
			seg16(data);
		}
		latch_high();
	}
}

void dot_on(int x, int y){ db[x/8 + 4*y] = 1 << (7-x%8); db_on(); }

void dot_off(int x, int y){ db[x/8 + 4*y] = 0x00; }



//void dot_on(x, y){;}
//void dot_off(x, y){;}

void seg_main(void) {
	init_dot();
//	seg_comm(0x8F);	// Duty 14/16,  Disp On
//
//	HAL_TIM_Base_Start_IT(&htim2);
//	HAL_TIM_Base_Start_IT(&htim3);
//	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);

//	for(int i=0; i<100; i++) sin_data[i] = 1000 * (1 + sin((PI*i)/50));

	while(1){
		for(int x=0; x<32; x++) {dot_on(x, 0); HAL_Delay(100); dot_off(x, 0);}
		for(int y=1; y<8; y++) {dot_on(31, y); HAL_Delay(100); dot_off(31, y);}
		for(int x=30; x>=0; x--) {dot_on(x, 7); HAL_Delay(100); dot_off(x, 7);}
		for(int y=6; y>0; y--) {dot_on(0, y); HAL_Delay(100); dot_off(0, y);}
//		seg_disp();
//		dig_key8(key[0], key[1], key[2], key[3]);
//		read();
//		keyboard();
	}
}

