#include "date.h"

int temp2 = 100;
int* MAX_SIZE2 = &temp2;
char display[100];

void getdate(){
	unsigned char month;
	unsigned char day;
	unsigned char year;
	char currentData[25];

	//set tempData and display to null
	int i;
	for (i=0;i<*MAX_SIZE2;i++){
		display[i]='\0';
	}
	for (i=0;i<25;i++){
		currentData[i]='\0';
	}
	
	
	//get month
	outb(0x70, 0x08);
	month = (unsigned char)inb(0x71);
	strcpy(currentData, intToAscii(bcdToInt(month)));
	sys_req(WRITE, DEFAULT_DEVICE, currentData, MAX_SIZE2);
	strcpy(currentData, "/ ");
	sys_req(WRITE, DEFAULT_DEVICE, currentData, MAX_SIZE2);		

	//get day
	outb(0x70, 0x07);
	day = (unsigned char)inb(0x71);
	strcpy(currentData, intToAscii(bcdToInt(day)));
	sys_req(WRITE, DEFAULT_DEVICE, currentData, MAX_SIZE2);	
	strcpy(currentData, "/ ");
	sys_req(WRITE, DEFAULT_DEVICE, currentData, MAX_SIZE2);

	//get year
	outb(0x70, 0x09);
	year = (unsigned char)inb(0x71);
	strcpy(currentData, intToAscii(bcdToInt(year)));
	sys_req(WRITE, DEFAULT_DEVICE, currentData, MAX_SIZE2);
	strcpy(currentData, "\n ");
	sys_req(WRITE, DEFAULT_DEVICE, currentData, MAX_SIZE2);	

}

void setDate(int month, int day, int year){
	char incorrectMonth[100] = "\x1B[31mInvalid Month\x1B[37m\n";
	char incorrectDay[100] = "\x1B[31mInvalid Day\x1B[37m\n";
	char incorrectYear[100] = "\x1B[31mInvalid Year\x1B[37m\n";
	unsigned char bcdData;
	//check if month is valid and return if it is not
	if(month < 1 || month > 12){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectMonth, MAX_SIZE2);
		return;
	}

	//check if day is valid and return if it is not
	if(day < 1 || day > 31){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectDay, MAX_SIZE2);
		return;
	}
	if(day == 31 && (month == 4 || month == 6 || month == 9 || month == 11)){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectDay, MAX_SIZE2);
		return;
	}
	if(month == 2 && day > 29 && year % 4 == 0){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectDay, MAX_SIZE2);
		return;
	}
	if(month == 2 && day > 28 && year % 4 != 0){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectDay, MAX_SIZE2);
		return;
	}

	//check in year is valid and return if it is not
	if(year < 0 || year > 99){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectYear, MAX_SIZE2);
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
