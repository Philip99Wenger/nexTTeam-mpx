#include "date.h"

void getdate();
int bcdToInt(unsigned char value);
void setDate(int month, int day, int year);
char * intToAscii(int integer)

int* MAX_SIZE=100;
char* display[MAX_SIZE];

void getdate(){
	unsigned char month;
	unsigned char day;
	unsigned char year;
	char currentData[25];
	char slash="/"

	//set tempData and display to null
	*currentData='\0';
	display='\0';
	
	//get month
	outb(0x70, 0x08)
	month = (unsigned char)inb(0x71);
	intToAscii(bcdToInt(month), currentData);
	strcat(display, currentData);
	strcat(display, slash);

	//get day
	outb(0x70, 0x07);
	day = (unsigned char)inb(0x71);
	intToAscii(bcdToInt(day), currentData);
	strcat(display, currentData);
	strcat(display, slash);

	//get year
	outb(0x70, 0x09);
	month = (unsigned char)inb(0x71);
	intToAscii(bcdToInt(year), currentData);
	strcat(display, currentData);

	sys_req(WRITE, DEFAULT_DEVICE, display, MAX_SIZE);

}

void setDate(int month, int day, int year){
	char* incorrectMonth[MAX_SIZE] = "Invalid Month";
	char* incorrectDay[MAX_SIZE] = "Invalid Day";
	char* incorrectYear[MAX_SIZE] = "Invalid Year";
	unsigned char bcdData;
	//check if month is valid and return if it is not
	if(month < 1 || month > 12){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectMonth, MAX_SIZE);
		return;
	}

	//check if day is valid and return if it is not
	if(day < 1 || month > 31){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectDay, MAX_SIZE);
		return;
	}

	//check in year is valid and return if it is not
	if(year < 0 || year > 99){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectYear, MAX_SIZE);
		return;
	}
	cli();
	
	//set year
	outb(0x70, 0x09);
	bcdData = intToBcd(year);
	outb(0x71, bcdData);

	//set month
	outb(0x70, 0x08);
	bcdData = intToBcd(month);
	outb(0x71, bcdData);


	//set day
	outb(0x70, 0x07);
	bcdData = intToBcd(day);
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
