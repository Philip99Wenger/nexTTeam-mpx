#include "time.h"

void gettime();
int bcdToInt(unsigned char value);
void setTime(int hours, int minutes, int seconds);
char * intToAscii(int integer)

int* MAX_SIZE=100;
char* display[MAX_SIZE];

void gettime(){
	unsigned char hours;
	unsigned char minutes;
	unsigned char seconds;
	char currentData[25];
	char colon=":"

	//set tempData and display to null
	*currentData='\0';
	display='\0';
	
	//get hours
	outb(0x70, 0x04)
	hours = (unsigned char)inb(0x71);
	intToAscii(bcdToInt(hours), currentData);
	strcat(display, currentData);
	strcat(display, colon);

	//get minutes
	outb(0x70, 0x02);
	minutes = (unsigned char)inb(0x71);
	intToAscii(bcdToInt(minutes), currentData);
	strcat(display, currentData);
	strcat(display, colon);

	//get seconds
	outb(0x70, 0x00);
	seconds = (unsigned char)inb(0x71);
	intToAscii(bcdToInt(seconds), currentData);
	strcat(display, currentData);

	sys_req(WRITE, DEFAULT_DEVICE, display, MAX_SIZE);

}

void setTime(int hours, int minutes, int seconds){
	char* incorrectHours[MAX_SIZE] = "Invalid Hours";
	char* incorrectMinutes[MAX_SIZE] = "Invalid Minutes";
	char* incorrectSeconds[MAX_SIZE] = "Invalid Seconds";
	unsigned char bcdData;
	//check if hours is valid and return if it is not
	if(hours < 1 || hours > 24){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectHours, MAX_SIZE);
		return;
	}

	//check if minutes is valid and return if it is not
	if(minutes < 0 || minutes > 59){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectMinutes, MAX_SIZE);
		return;
	}

	//check in seconds is valid and return if it is not
	if(seconds < 0 || seconds > 59){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectSecconds, MAX_SIZE);
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

unsigned char intToBcd(int value){
}
	 
}
