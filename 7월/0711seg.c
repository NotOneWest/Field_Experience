#include "main.h"
#include "glcs5x7.h"
#include "string.h"

extern TIM_HandleTypeDef htim2;

#define data_high() 	data_GPIO_Port->BSRR=data_Pin
#define data_low() 		data_GPIO_Port->BRR=data_Pin
#define read_data()     ((data_GPIO_Port->IDR)&(data_Pin))
#define shift_high() 	shift_GPIO_Port->BSRR=shift_Pin
#define shift_low() 	shift_GPIO_Port->BRR=shift_Pin
#define latch_high() 	latch_GPIO_Port->BSRR=latch_Pin
#define latch_low() 	latch_GPIO_Port->BRR=latch_Pin

unsigned char db[64]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

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
	static unsigned int index, data;

	index = (index<65) ? index+1 : 0;

	for(int y=0; y<8; y++){
		latch_low();
		for(int x=0; x<4; x++){
			data = ((y+1) << 8) | db[4*y + x];
			seg16(data);
		}
		latch_high();
	}
}

void move(){
	static int time;

	time++;

	if(time%10000 == 0) {
		for(int y=0; y<8; y++){
			unsigned char temp=db[0+4*y];
			db[0+4*y] = (db[0+4*y] << 1) | (db[1+4*y] >> 7);
			db[1+4*y] = (db[1+4*y] << 1) | (db[2+4*y] >> 7);
			db[2+4*y] = (db[2+4*y] << 1) | (db[3+4*y] >> 7);
			db[3+4*y] = (db[3+4*y] << 1) | (db[32+4*y] >> 7);
			db[32+4*y] = (db[32+4*y] << 1) | (db[33+4*y] >> 7);
			db[33+4*y] = (db[33+4*y] << 1) | (db[34+4*y] >> 7);
			db[34+4*y] = (db[34+4*y] << 1) | (db[35+4*y] >> 7);
			db[35+4*y] = (db[35+4*y] << 1) | (temp >> 7);
		}
		db_on();
	}
}

void dot_on(int x, int y){ db[x/8 + 4*y] |= 1 << (7-x%8); }

void dot_off(int x, int y){ db[x/8 + 4*y] &= ~(1 << (7-x%8)); }

void text_on(char *text){
	int idx=0, len=strlen(text);
	for(int t=0; t<len; t++){
		int start = (text[t] - ' ') * 5;
		int end = start + 5;
		for(int i=start; i<end; i++){
			for(int j=0; j<8; j++){
				int x, y;
				if((idx+(i-start)) > 31) {x=(idx+(i-start)-32); y=8;}
				else {x=(idx+(i-start)); y=0;}

				if(efn[i] & (1<<j)) dot_on(x, y+j);
				else dot_off(x, y+j);
			}
		}
		idx+=6;
	}
}

void seg_main(void) {
	HAL_TIM_Base_Start_IT(&htim2);

	init_dot();
	text_on("An Hanseo");

	while(1){
//		for(int x=0; x<32; x++) {dot_on(x, 0); HAL_Delay(100); dot_off(x, 0);}
//		for(int y=1; y<8; y++) {dot_on(31, y); HAL_Delay(100); dot_off(31, y);}
//		for(int x=30; x>=0; x--) {dot_on(x, 7); HAL_Delay(100); dot_off(x, 7);}
//		for(int y=6; y>0; y--) {dot_on(0, y); HAL_Delay(100); dot_off(0, y);}

	}
}

