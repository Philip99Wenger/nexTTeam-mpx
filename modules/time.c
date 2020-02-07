#include "time.h"

int temp1 = 100;
int* MAX_SIZE1 = &temp1;

char display[100];

void gettime(){
	unsigned char hours;
	unsigned char minutes;
	unsigned char seconds;
	char currentData[25];

	//set tempData and display to null
	int i;
	for (i=0;i<*MAX_SIZE1;i++){
		display[i]='\0';
	}
	for (i=0;i<25;i++){
		currentData[i]='\0';
	}
	
	//get hours
	outb(0x70, 0x04);
	hours = (unsigned char)inb(0x71);
	strcpy(currentData, intToAscii(bcdToInt(hours)));
	sys_req(WRITE, DEFAULT_DEVICE, currentData, MAX_SIZE1);
	strcpy(currentData, ": ");
	sys_req(WRITE, DEFAULT_DEVICE, currentData, MAX_SIZE1);	

	//get minutes
	outb(0x70, 0x02);
	minutes = (unsigned char)inb(0x71);
	strcpy(currentData, intToAscii(bcdToInt(minutes)));
	sys_req(WRITE, DEFAULT_DEVICE, currentData, MAX_SIZE1);
	strcpy(currentData, ": ");
	sys_req(WRITE, DEFAULT_DEVICE, currentData, MAX_SIZE1);	

	//get seconds
	outb(0x70, 0x00);
	seconds = (unsigned char)inb(0x71);
	strcpy(currentData, intToAscii(bcdToInt(seconds)));
	sys_req(WRITE, DEFAULT_DEVICE, currentData, MAX_SIZE1);
	strcpy(currentData, "\n ");
	sys_req(WRITE, DEFAULT_DEVICE, currentData, MAX_SIZE1);	
}

void setTime(int hours, int minutes, int seconds){
	char incorrectHours[100] = "\x1B[31mInvalid Hours\x1B[37m\n";
	char incorrectMinutes[100] = "\x1B[31mInvalid Minutes\x1B[37m\n";
	char incorrectSeconds[100] = "\x1B[31mInvalid Seconds\x1B[37m\n";
	unsigned char bcdData;
	//check if hours is valid and return if it is not
	if(hours < 1 || hours > 24){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectHours, MAX_SIZE1);
		return;
	}

	//check if minutes is valid and return if it is not
	if(minutes < 0 || minutes > 59){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectMinutes, MAX_SIZE1);
		return;
	}

	//check in seconds is valid and return if it is not
	if(seconds < 0 || seconds > 59){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectSeconds, MAX_SIZE1);
		return;
	}
	cli();
	
	//set hours
	outb(0x70, 0x04);
	bcdData = intToBcd(hours);
	outb(0x71, bcdData);

	//set minutes
	outb(0x70, 0x02);
	bcdData = intToBcd(minutes);
	outb(0x71, bcdData);


	//set seconds
	outb(0x70, 0x00);
	bcdData = intToBcd(seconds);
	outb(0x71, bcdData);


	sti();

}

int bcdToInt(unsigned char value){
	int integerValue;
	integerValue = (int)((value & 0xF0) >> 4);
	integerValue = integerValue * 10;
	integerValue = (int)(value & 0x0F) + integerValue;
	return integerValue;
}

unsigned char intToBcd(int integer){
	//Split the Digits
	int ones = integer%10;
	integer/=10;
	int tens = integer%10;
	
	//Fill the Char
	unsigned char bcdValue = (tens << 4) + ones;
	return bcdValue;
}

