#include "main.h"
#include "glcs5x7.h"
#include "string.h"

extern TIM_HandleTypeDef htim2;
extern I2C_HandleTypeDef hi2c1;

#define data_high() 	data_GPIO_Port->BSRR=data_Pin
#define data_low() 		data_GPIO_Port->BRR=data_Pin
#define clk_high() 		clk_GPIO_Port->BSRR=clk_Pin
#define clk_low() 		clk_GPIO_Port->BRR=clk_Pin

#define read_data()     ((data_GPIO_Port->IDR)&(data_Pin))
#define read_ROTA()     ((ROTA_GPIO_Port->IDR)&(ROTA_Pin))
#define read_ROTB()     ((ROTB_GPIO_Port->IDR)&(ROTB_Pin))
#define read_ROTSW()    ((ROTSW_GPIO_Port->IDR)&(ROTSW_Pin))

unsigned char db[17]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned int text_db[100] = {0, };
static int len, position;

void init_dot(){
	unsigned char init_db[3]={0x21, 0x81, 0xE1};
	for(int i=0; i<3; i++) HAL_I2C_Master_Transmit(&hi2c1, 0xE0, init_db+i, 1, 1);
}

void show_db() { HAL_I2C_Master_Transmit(&hi2c1, 0xE0, db, 17, 1); }

void dot_on(int x, int y){ db[x/8 + 2*y + 1] |= 1 << (x%8); }

void dot_off(int x, int y){ db[x/8 + 2*y + 1] &= ~(1 << (x%8)); }

void write_text(char *text, int len){
	for(int idx=0; idx<len; idx++){
		int start = (text[idx] - ' ') * 5;
		for(int loc=0; loc<5; loc++) text_db[loc + 6*idx] = efn[start + loc];
		text_db[5 + 6*idx] = 0x00;
	}
}

void move(){

	static int time;

	time++;

	if(time%100 == 0) {
//		static int start;
		while(position < 0) position += (len*6);

		for(int x=0; x<16; x++){
			for (int y=0; y<8; y++){
				if(text_db[x+(position % (len*6))] & (1<<y)) dot_on(x, y);
				else dot_off(x, y);
			}
		}
		show_db();
//		start++;
	}
}

int read_rot(){
	if(read_ROTA() && read_ROTB()) return 3;
	if(~read_ROTA() && read_ROTB()) return 2;
	if(read_ROTA() && ~read_ROTB()) return 1;
	return 0;
}

void seg_main(void) {
	static int rot, old_rot;

	HAL_TIM_Base_Start_IT(&htim2);

	init_dot();

	char *text = "An Hanseo";
	len=strlen(text);
	write_text(text, len);


	while(1){
		rot = read_rot();
		switch(old_rot){
			case 0:
				if(rot == 1) position++;
				else if(rot == 2) position--;
				break;
			case 1:
				if(rot == 3) position++;
				else if(rot == 0) position--;
				break;
			case 2:
				if(rot == 0) position++;
				else if(rot == 3) position--;
				break;
			case 3:
				if(rot == 2) position++;
				else if(rot == 1) position--;
				break;
			default:
				break;
		}
		old_rot = rot;



	}
}

