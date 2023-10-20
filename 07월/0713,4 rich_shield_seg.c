#include "main.h"
#include "glcs5x7.h"
#include "string.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim6;
extern I2C_HandleTypeDef hi2c1;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

// Battery led
void delay(int i)  {	while(i) i--; }
void bat_clk_high(void)  { clk_GPIO_Port->BSRR=clk_Pin; delay(200); }
void bat_clk_low(void)   { clk_GPIO_Port->BRR=clk_Pin; delay(200); }
void bat_data_high(void) { data_GPIO_Port->BSRR=data_Pin; delay(200); }
void bat_data_low(void)  { data_GPIO_Port->BRR=data_Pin; delay(200); }
void bat_start(void) {  bat_data_high(); bat_clk_high(); bat_data_low(); }
void bat_stop(void)  {  bat_data_low();  bat_clk_high(); bat_data_high(); }
// RGB led
#define rgb_high() 			RGB_GPIO_Port->BSRR=RGB_Pin
#define rgb_low() 			RGB_GPIO_Port->BRR=RGB_Pin
// Seven segment
#define seg_data_high() 	seg_data_GPIO_Port->BSRR=seg_data_Pin
#define seg_data_low() 		seg_data_GPIO_Port->BRR=seg_data_Pin
#define shift_high() 		shift_GPIO_Port->BSRR=shift_Pin
#define shift_low() 		shift_GPIO_Port->BRR=shift_Pin
#define latch_high() 		latch_GPIO_Port->BSRR=latch_Pin
#define latch_low() 		latch_GPIO_Port->BRR=latch_Pin
// Rotary encoder
#define read_ROTA()     	((ROTA_GPIO_Port->IDR)&(ROTA_Pin))
#define read_ROTB()     	((ROTB_GPIO_Port->IDR)&(ROTB_Pin))
#define read_ROTSW()    	((ROTSW_GPIO_Port->IDR)&(ROTSW_Pin))
// DS18B20
void temp_high() 			{ temp_GPIO_Port->BSRR=temp_Pin; }
#define temp_low() 			temp_GPIO_Port->BRR=temp_Pin
#define read_temp()    		((temp_GPIO_Port->IDR)&(temp_Pin))

unsigned char seg_db[8] = {0x03, 0x9f, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f};
const unsigned int address[8] = {0x0800, 0x0400, 0x0200, 0x0100, 0x8000, 0x4000, 0x2000, 0x1000};
const unsigned char font[] = {0x03, 0x9f, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f, 0x01, 0x09, 0x11, 0xC1, 0x63, 0x85, 0x61, 0x71}; // 0~9, A~F

unsigned char dot_db[17]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned int text_db[100] = {0, };
int RGB[4]={0x00000000, 0x00000000, 0x00000000, 0x00000000};
static int position, X, dir, degree;
uint8_t ADCvalue[3];

void digHex4(int x, int y){
	seg_db[0] = font[(x >> 4)];
	seg_db[1] = font[(x & 0x0f)];
	seg_db[2] = font[(y >> 4)];
	seg_db[3] = font[(y & 0x0f)];
}

void digTemp(int degree){
	seg_db[4] = font[ degree /1000 ];
	seg_db[5] = (font[ ( degree % 1000 )/100 ] - 0x01);
	seg_db[6] = font[ ( degree % 100 ) /10 ];
	seg_db[7] = font[ degree % 10 ];
}

void disp_seg(void){
	static unsigned int data, j;
	j = (j<7) ? j+1 : 0;
	data = address[j] | seg_db[j];

	latch_low();
	for(int i=0; i<16; i++){
		shift_low();
		if(data & (1 << i)) { seg_data_high();}	else{seg_data_low();}
		shift_high();
	}
	latch_high();
}

void init_dot(){
	unsigned char init_db[3]={0x21, 0x81, 0xE1};
	for(int i=0; i<3; i++) HAL_I2C_Master_Transmit(&hi2c1, 0xE0, init_db+i, 1, 1);
}

void show_db() { HAL_I2C_Master_Transmit(&hi2c1, 0xE0, dot_db, 17, 5); }

void dot_on(int x, int y){ dot_db[x/8 + 2*y + 1] |= 1 << (x%8); }

void dot_off(int x, int y){ dot_db[x/8 + 2*y + 1] &= ~(1 << (x%8)); }

void erase_dot(){ for(int i=1; i<=16; i++) dot_db[i] = 0x00; }

void rgb_out(void){
	int i,j,k,x;
	for (x=0;x<4;x++){
		k=0x00800000;
		j=RGB[x];
		while(k){
			if (k & j){
				rgb_high();		for(i=0;i<3;i++);
				rgb_low();		for(i=0;i<2;i++);
			} else{
				rgb_high();		for(i=0;i<1;i++);
				rgb_low();		for(i=0;i<3;i++);
			}
			k = k >> 1 ;
		}
	}
	rgb_low(); for(i=0;i<300;i++);
}

void control_RGB(){
	static int flag=1;

	if(position < 0) position=0;
	if(position > 128) position=127;

	if(read_ROTSW()) {
		if(flag) {
			flag=0; X++;
			if(X==3) X=4;
			if(X==7) X=0;
		}
		position=0;
		if(X==0) erase_dot();
	} else flag=1;

	static int Y;
	Y = (7-(position/16)%8);

	if(dir==1){ for(int i=0; i<Y; i++) dot_off(X, i);}
	dot_on(X, Y);
	show_db();

	if(X==0) for(int i=0; i<4; i++) RGB[i] = 0x00000000;
	if(X==1) {RGB[0] &= (0xff01ff); RGB[1] &= (0xff01ff);}
	if(X==2) {RGB[0] &= (0xffff01); RGB[1] &= (0xffff01);}
	if(X==4) {RGB[2] &= (0x01ffff); RGB[3] &= (0x01ffff);}
	if(X==5) {RGB[2] &= (0xff01ff); RGB[3] &= (0xff01ff);}
	if(X==6) {RGB[2] &= (0xffff01); RGB[3] &= (0xffff01);}

	int bright=0x00;
	for(int i=1; i<17; i+=2){
		if( ((dot_db[16-i] >> X) & 1) ) bright |= (1 << (i/2));
	}

	if(X<3){
		RGB[0] |= (bright << (8*(2-X)));
		RGB[1] |= (bright << (8*(2-X)));
	} else {
		RGB[2] |= (bright << (8*(6-X)));
		RGB[3] |= (bright << (8*(6-X)));
	}
}

unsigned char BAT_LED=0x55;
void bat_tr(int tdata){
	int i,k=0x01;
	for (i=0;i<8;i++){
		bat_clk_low();
		if (tdata & k) bat_data_high();
		else bat_data_low();
		bat_clk_high();
		k = k << 1;
	}
	bat_clk_low(); bat_data_high(); bat_clk_high(); bat_clk_low();
}

void bat_disp(int disp) { bat_start(); bat_tr(0xC0); bat_tr(disp); bat_stop();}

int read_rot(){
	int rot=0;
	if(read_ROTA()) rot++;
	if(read_ROTB()) rot+=2;
	return rot;
}

void call_DMA(){
	static int time;
	time++;

	if(time%500 == 0){
		digHex4(ADCvalue[0], ADCvalue[1]);
		digTemp(degree);

		if(ADCvalue[2] > 224) bat_disp(0x7F);
		if(ADCvalue[2] > 192) bat_disp(0x7E);
		if(ADCvalue[2] > 160) bat_disp(0x7C);
		if(ADCvalue[2] > 128) bat_disp(0x78);
		if(ADCvalue[2] > 96) bat_disp(0x70);
		if(ADCvalue[2] > 64) bat_disp(0x60);
		if(ADCvalue[2] > 32) bat_disp(0x40);
		if(ADCvalue[2] <= 32) bat_disp(0x80);
	}
}

void delay_us (uint16_t us) {
	__HAL_TIM_SET_COUNTER(&htim6,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim6) < us);  // wait for the counter to reach the us input in the parameter
}

uint8_t DS18B20_Start (void)
{
	uint8_t Response = 0;
	temp_low();
	delay_us(480);   // delay according to datasheet

	temp_high();    // set the pin as input
	delay_us(80);    // delay according to datasheet

	if (!(read_temp())) Response = 1;    // if the pin is low i.e the presence pulse is detected
	else Response = -1;

	delay_us(400); // 480 us delay totally.

	return Response;
}

void DS18B20_Write (uint8_t data){
	temp_low();
	for (int i=0; i<8; i++){
		if ((data & (1<<i))!=0){ // if the bit is high
			// write 1
			temp_low();  // pull the pin LOW
			delay_us(1);  // wait for 1 us

			temp_high(); // set as input
			delay_us(50);  // wait for 60 us
		} else{  // if the bit is low
			// write 0
			temp_low();  // pull the pin LOW
			delay_us(50);  // wait for 60 us

			temp_high();
		}
	}
}

uint8_t DS18B20_Read (void){
	uint8_t value=0;
	temp_high();

	for (int i=0;i<8;i++){
		temp_low(); // pull the data pin LOW
		delay_us(2);  // wait for 2 us

		temp_high();  // set as input
		// if the pin is HIGH
		if (read_temp()) value |= 1<<i;  // read = 1
		delay_us(60);  // wait for 60 us
	}
	return value;
}

uint8_t Presence, Temp_byte1, Temp_byte2;
void DS18B20_control(){
	Presence = DS18B20_Start ();
	HAL_Delay (1);
	DS18B20_Write (0xCC);  // skip ROM
	DS18B20_Write (0x44);  // convert t
	HAL_Delay (1);

	Presence = DS18B20_Start ();
	HAL_Delay(1);
	DS18B20_Write (0xCC);  // skip ROM
	DS18B20_Write (0xBE);  // Read Scratch-pad

	Temp_byte1 = DS18B20_Read();
	Temp_byte2 = DS18B20_Read();
	double TEMP = (Temp_byte2 << 8) | Temp_byte1;
	degree = (double) (TEMP*100)/16;
}

void seg_main(void) {
	static int rot, old_rot;

	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim6);

	HAL_ADC_Start_DMA(&hadc1, (uint8_t *)ADCvalue, 3);

	init_dot();

	bat_start(); bat_tr(0x8C); bat_stop();
	bat_start(); bat_tr(0x40); bat_stop();

	while(1){
		rot = read_rot();
		switch(old_rot){
			case 0:
				if(rot == 1) {position++; dir=0;}
				else if(rot == 2) {position--; dir=1;}
				break;
			case 1:
				if(rot == 3) {position++; dir=0;}
				else if(rot == 0) {position--; dir=1;}
				break;
			case 2:
				if(rot == 0) {position++; dir=0;}
				else if(rot == 3) {position--; dir=1;}
				break;
			case 3:
				if(rot == 2) {position++; dir=0;}
				else if(rot == 1) {position--; dir=1;}
				break;
			default:
				break;
		}
		old_rot = rot;
		control_RGB();
		DS18B20_control();
	}
}

