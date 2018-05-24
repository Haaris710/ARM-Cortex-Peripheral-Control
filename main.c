#include <lpc17xx.h>



//Exercise 1: LEDs
void ledDisplay(uint8_t num) {
	//USEFUL CONSTANTS
	const uint32_t ALL_GPIO1 = 0x58000000;
	const uint32_t ALL_GPIO2 = 0x0000007C;
	
	//VARIABLES
	uint32_t write = 0;
	uint32_t extract = 0;
	
	if(num > 255 || num < 0)
		return;
	
	//INITIALIZATION
	LPC_GPIO1->FIODIR |= 0xB0000000;
	LPC_GPIO2->FIODIR |= 0x0000007C;
	
	//CLEAR ALL
	LPC_GPIO1->FIOCLR |= ALL_GPIO1;
	LPC_GPIO2->FIOCLR |= ALL_GPIO2;
	
	//WRITE TO LED
	LPC_GPIO1->FIOSET |= ((num & 0x80) >> 7) << 28);
	LPC_GPIO1->FIOSET |= ((num & 0x40) >> 6) << 29);
	LPC_GPIO1->FIOSET |= ((num & 0x20) >> 5) << 31);
	//LPC_GPIO1->FIOSET = write;
	
	//write = 0;
	LPC_GPIO2->FIOSET |= ((num & 0x10) >> 4) << 6);
	LPC_GPIO2->FIOSET |= ((num & 0x08) >> 3) << 5);
	LPC_GPIO2->FIOSET |= ((num & 0x04) >> 2) << 4);
	LPC_GPIO2->FIOSET |= ((num & 0x02) >> 1) << 2);
	LPC_GPIO2->FIOSET |= ((num & 0x01) << 2);
	//LPC_GPIO2->FIOSET = write;
	
	
	
}

int main(void) {
	
	uint8_t number = 10;
	ledDisplay(number);
	
	
	while(1);
	
}