#include <lpc17xx.h>
#include <stdio.h>
#include <stdint.h>
#include "GLCD.h"
#include <string.h>
#include <RTL.h>

#define BIT0 (0x1)
#define BIT1 (0x1 << 1)
#define BIT2 (0x1 << 2)
#define BIT3 (0x1 << 3)
#define BIT4 (0x1 << 4)
#define BIT5 (0x1 << 5)
#define BIT6 (0x1 << 6)
#define BIT7 (0x1 << 7)
#define BIT10 (0x1 << 10)
#define BIT12 (0x1 << 12)
#define BIT18 (0x1 << 18)
#define BIT20 (0x1 << 20)
#define BIT23 (0x1 << 23)
#define BIT24 (0x1 << 24)
#define BIT25 (0x1 << 25)
#define BIT26 (0x1 << 26)
#define BIT31 (0x1 << 31)

static uint8_t ledState = 0;


//Exercise 1: LEDs
void ledDisplay(uint8_t num) {
	//NOTE: Bit shifiting right/left not combined to
	//maintain readability
	
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
	buffer |= ((num & BIT7) >> 7) << 28;
	buffer |= ((num & BIT6) >> 6) << 29;
	buffer |= ((num & BIT5) >> 5) << 31;
	
	//Write to register for GPIO1
	LPC_GPIO1->FIOSET |= buffer;
	buffer = 0;

	//Build buffer for GPIO2
	buffer |= ((num & BIT4) >> 4) << 2;
	buffer |= (num & BIT3); //(Right shift three, left shift three)
	buffer |= ((num & BIT2) >> 2) << 4;
	buffer |= ((num & BIT1) >> 1) << 5;
	buffer |= (num & BIT0) << 6; 
	
	//Write to register for GPIO1
	LPC_GPIO2->FIOSET |= buffer;
	buffer = 0;
	ledState = num;
}

//EXERCISE 2: Joystick
void joyStickReadToPutty(void) {
	//VARIABLES
	uint32_t buffer = 0;
	
	//Read from GPIO1
	buffer |= LPC_GPIO1->FIOPIN;
	
	if((buffer & BIT23) == 0) {
		printf("Up");
  }
	else if((buffer & BIT24) == 0) {
		printf("Right");
	}
	else if((buffer & BIT25) == 0) {
		printf("Down");
	}
	else if((buffer & BIT26) == 0) {
		printf("Left");
	}
	else
		printf("NO DIR");
	printf("\r");
	
	if((buffer & BIT20) == 0)
		printf("Pressed");
	else
		printf("Not Pressed");
	
	printf("\r\n");
}

void joyStickReadToConsole(void) {
	//VARIABLES
	uint32_t buffer = 0;
	
	//Read from GPIO1
	buffer |= LPC_GPIO1->FIOPIN;
	
	if((buffer & BIT23) == 0) {
		printf("Up");
  }
	else if((buffer & BIT24) == 0) {
		printf("Right");
	}
	else if((buffer & BIT25) == 0) {
		printf("Down");
	}
	else if((buffer & BIT26) == 0) {
		printf("Left");
	}
	else
		printf("NO DIR");
	printf("\r");
	
	if((buffer & BIT20) == 0)
		printf("Pressed");
	else
		printf("Not Pressed");
	printf("\r\r");
}

uint8_t joyStickRead(void) {
	//VARIABLES
	uint32_t buffer = 0;
	uint8_t output = 0;
	
	//Read from GPIO1
	buffer |= LPC_GPIO1->FIOPIN;
	
	if((buffer & BIT23) == 0) { //up
		output = 0x01;
  }
	else if((buffer & BIT24) == 0) { //right
		output = 0x04;
	}
	else if((buffer & BIT25) == 0) { //down
		output = 0x02;
	}
	else if((buffer & BIT26) == 0) { //left
		output = 0x03;
	}
	else
		output = 0;
	
	if((buffer & BIT20) == 0) //pressed
		output |= (0x01 << 4);
	
	return output;
}

void pBtnReadConsole(void) {
	//VARIABLES
	uint32_t buffer = 0;
	
	//Read from GPIO2
	buffer |= LPC_GPIO2->FIOPIN;
	
	if((buffer & BIT10) == 0)
		printf("Push Button Pressed\r\n");
}

uint8_t pBtnRead(void) {
	//VARIABLES
	uint32_t buffer = 0;
	
	//Read from GPIO2
	buffer |= LPC_GPIO2->FIOPIN;
	
	if((buffer & BIT10) == 0)
		return 1;
	return 0;
}


//EXERCISE 4: Potentiometer
void potentiometerRead(void) {
	//INITIALIZATION
	uint32_t buffer = 0;
	
	//clear bit 18,19 (provided code)
	LPC_PINCON->PINSEL1 &= ~(0x03 << 18); 
	//set bit 18
	LPC_PINCON->PINSEL1 |= BIT18; 
	//set bit 12 to apply power
	LPC_SC->PCONP |= BIT12;
	//select AD0.2 pin, ADC clock 25MHz/5, enable ADC (provided code)
	LPC_ADC->ADCR = (1 << 2) | (4 << 8) | (1 << 21);
	
	
	//Start conversion
	LPC_ADC->ADCR |= BIT24;
	
	//Wait for conversion to complete
	while((LPC_ADC->ADGDR & BIT31) > 0) {
		printf("Waiting for conversion\r\n");
	}
	
	//Read from ADGDR register and shift 4 bits right
	buffer |= (LPC_ADC->ADGDR >> 4);
	
	//Extract 12 bits on the LSB side
	buffer &= 0xFFF;
	
	//Print to console
	printf("Potentiometer: %d\r\n",buffer);
}

void lcdDisplay(char* string, uint8_t line) {
	GLCD_ClearLn(line,1);
	GLCD_DisplayString(line,1,1, string);
}

void lcdInit() {
	//INITIALIZATION
	GLCD_Init();
	GLCD_Clear(Blue);
	GLCD_SetBackColor(Blue);
	GLCD_SetTextColor(White);
}


__task void joy_task(void) {
	uint8_t state = 0;
	char* output;
	lcdInit();
	while(1) {
		state = joyStickRead();
		if((state & 0x07) == 0)
			output = "NO DIR";
		else if((state & 0x07) == 0x01)
			output = "UP";
		else if((state & 0x07) == 0x02)
			output = "DOWN";
		else if((state & 0x07) == 0x03)
			output = "LEFT";
		else if((state & 0x07) == 0x04)
			output = "RIGHT";
		else output = "void";
		
		lcdDisplay(output, 1);
		
		if((state & (0x01 << 4)) > 0)
			output = "PRESSED";
		else
			output = "NOT PRESSED";
		
		lcdDisplay(output, 2);
		os_tsk_pass();
	}
}

__task void poten_task(void) {
	while(1) {
		potentiometerRead();
		os_tsk_pass();
	}
}

__task void led_task(void) {
	//Initialize LED to zero
	ledDisplay(0);
	ledState = 0;
	while(1) {
		//Wait for press and release
		while(pBtnRead() != 1) {}
		while(pBtnRead() != 0) {}
		if(ledState == 1) {
			ledDisplay(0);
			ledState = 0;
		}
		else {
			ledDisplay(1);
			ledState = 1;
		}
		os_tsk_pass();
	}
}

__task void init_tasks(void) {
	os_tsk_create(poten_task,1);
	os_tsk_create(joy_task,1);
	os_tsk_create(led_task,1);
	
	while(1) {}
}


/*
//LED TEST
int main(void) {
	SystemInit();

	ledDisplay(123);
	while(1);
}
*/


/*
//JOYSTICK TEST
int main(void) {
	SystemInit();
	
	while(1) {
		joyStickReadToPutty();
  }
}
*/

/*
//PUSH BUTTON TEST
int main(void) {
	SystemInit();
	
	while(1) {
		pBtnReadConsole();
	}
*/

/*
//POTENTIOMETER TEST
int main(void) {
	SystemInit();
	
	while(1)
		potentiometerRead();
}
*/


/*
//LCD TEST
int main(void) {
	char* string  = "Hello World";
	SystemInit();
	lcdInit();
	
	lcdDisplay(string, 1);
}
*/

/*
int main(void) {
	char* string = "hello";
	SystemInit();
	joy_task();
	while(1) {}
}
*/


//TASKS IMPLEMENTATION TEST
int main(void) {
	printf("Task Implementation Test");
	SystemInit();
	os_sys_init(init_tasks);
}











