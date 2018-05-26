#include <lpc17xx.h>
#include <stdint.h>

//Exercise 1: LEDs
void ledDisplay(uint8_t num) {
	//USEFUL CONSTANTS
	const uint32_t ALL_LED_GPIO1 = 0x58000000;
	const uint32_t ALL_LED_GPIO2 = 0x0000007C;
	
	//VARIABLES
	uint32_t buffer = 0;
	
	//Check for invalid input
	if(num > 255 || num < 0)
		return;
	
	//Initialization
	LPC_GPIO1->FIODIR |= 0xB0000000;
	LPC_GPIO2->FIODIR |= 0x0000007C;
	
	//Clear all LEDs
	LPC_GPIO1->FIOCLR |= ALL_LED_GPIO1;
	LPC_GPIO2->FIOCLR |= ALL_LED_GPIO2;
	
	//Build buffer for GPIO1
	buffer |= ((num & 0x80) >> 7) << 28;
	buffer |= ((num & 0x40) >> 6) << 29;
	buffer |= ((num & 0x20) >> 5) << 31;
	
	//Write to register for GPIO1
	LPC_GPIO1->FIOSET |= buffer;
	buffer = 0;

	//Build buffer for GPIO2
	buffer |= ((num & 0x10) >> 4) << 2;
	buffer |= (num & 0x08); // >> 3) << 3
	buffer |= ((num & 0x04) >> 2) << 4;
	buffer |= ((num & 0x02) >> 1) << 5;
	buffer |= (num & 0x01) << 6; 
	
	//Write to register for GPIO1
	LPC_GPIO2->FIOSET |= buffer;
	buffer = 0;
}

int main(void) {
	
	ledDisplay(42);
	
	/*
	//USEFUL CONSTANTS
	const uint32_t ALL_LED_GPIO1 = 0x58000000;
	const uint32_t ALL_LED_GPIO2 = 0x0000007C;
	uint32_t testnumber = 0;
	
	//Initialization
	LPC_GPIO1->FIODIR |= 0xB0000000;
	LPC_GPIO2->FIODIR |= 0x0000007C;
	
	LPC_GPIO1->FIOCLR |= ALL_LED_GPIO1;
	LPC_GPIO2->FIOCLR |= ALL_LED_GPIO2;
	
	testnumber |= (0x000001 << 29);
	LPC_GPIO1->FIOSET |= testnumber;
	*/
	
	while(1);
	
}