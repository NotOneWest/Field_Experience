#include "main.h"

#define data_high() 	data_GPIO_Port->BSRR=data_Pin
#define data_low() 		data_GPIO_Port->BRR=data_Pin
#define shift_high() 	shift_GPIO_Port->BSRR=shift_Pin
#define shift_low() 	shift_GPIO_Port->BRR=shift_Pin
#define latch_high() 	latch_GPIO_Port->BSRR=latch_Pin
#define latch_low() 	latch_GPIO_Port->BRR=latch_Pin

void trans16(int data) {
	latch_low();
	shift_low();

	for(int i=0; i<16; i++){
		if((data & (1 << i)) != 0) data_low();
		else data_high();

		shift_high();
		shift_low();
	}
	latch_high();
}

void seg_main(void) {
	const int test[9] = {0xf760, 0xfbda, 0xfdf2, 0xfe66, 0x7fb6, 0xbfbe, 0xdfe0, 0xeffe, 0xf7f6}; // 0xf7f6 (1번, 9)
	int index = 0;

	while(1) {
		if(index < 8) index++;
		else index=0;

		trans16(test[index]);
		HAL_Delay(500);
	}
}

