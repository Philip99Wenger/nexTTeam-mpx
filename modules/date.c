#include "date.h"

int temp2 = 100;
int* MAX_SIZE2 = &temp2;
char display[100];

void getdate(){
	unsigned char month;
	unsigned char day;
	unsigned char year;
	char currentData[25];
	char slash[2]="/ ";

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
	strcpy(currentData, intToAscii(bcdToInt1(month)));
	strcat(display, currentData);
	strcat(display, slash);

	//get day
	outb(0x70, 0x07);
	day = (unsigned char)inb(0x71);
	strcpy(currentData, intToAscii(bcdToInt1(day)));
	strcat(display, currentData);
	strcat(display, slash);

	//get year
	outb(0x70, 0x09);
	year = (unsigned char)inb(0x71);
	strcpy(currentData, intToAscii(bcdToInt1(year)));
	strcat(display, currentData);

	sys_req(WRITE, DEFAULT_DEVICE, display, MAX_SIZE2);

}

void setDate(int month, int day, int year){
	char incorrectMonth[100] = "Invalid Month";
	char incorrectDay[100] = "Invalid Day";
	char incorrectYear[100] = "Invalid Year";
	unsigned char bcdData;
	//check if month is valid and return if it is not
	if(month < 1 || month > 12){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectMonth, MAX_SIZE2);
		return;
	}

	//check if day is valid and return if it is not
	if(day < 1 || month > 31){
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
	bcdData = intToBcd1(year);
	outb(0x71, bcdData);

	//set month
	outb(0x70, 0x08);
	bcdData = intToBcd1(month);
	outb(0x71, bcdData);


	//set day
	outb(0x70, 0x07);
	bcdData = intToBcd1(day);
	outb(0x71, bcdData);


	sti();

}

int bcdToInt1(unsigned char value){
	int integerValue;
	integerValue = (int)((value & 0xF0) >> 4);
	integerValue = integerValue * 10;
	integerValue = (int)(value & 0x0F) + integerValue;
	return integerValue;
}

unsigned char intToBcd1(int value){
	//Split the Digits
	int ones = integer%10;
	integer/=10;
	int tens = integer%10;
	
	//Fill the Char
	unsigned char bcdValue = (tens << 4) + ones;
	return bcdValue;
}
