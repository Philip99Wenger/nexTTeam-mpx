#include "r4commands.h"

int possibleAlarms = 100;//This contains the total alarms possible
char alarmList[possibleAlarms][100];//This contains all the active alarms
char alarmMessages[possibleAlarms][100];//This contains all the messages of the active alarms
int totalAlarms = 0;//This contains the amount of active alarms

void setAlarm(char timeStatement[], char message[]){
	//Error & Success Messages	
	char incorrectHours[100] = "\x1B[31mInvalid Hours\x1B[37m\n";
	char incorrectMinutes[100] = "\x1B[31mInvalid Minutes\x1B[37m\n";
	char incorrectSeconds[100] = "\x1B[31mInvalid Seconds\x1B[37m\n";
	char success[] = "\x1B[32mSuccessfully set the alarm. \x1B[37m\n";
	int successSize = strlen(success);
	int falseHoursSize = strlen(incorrectHours);
	int falseMinutesSize = strlen(incorrectMinutes);
	int falseSecondsSize = strlen(incorrectSeconds);
	//Variables
	int nextIndex = 0;
	char alarmPcbName[] = "alarm";
	int parArr[3];
	int i=0;
	
	//Divide time statement into components
	char* token = strtok(timeStatement, ":");
	while (token != NULL){
		parArr[i] = atoi(token);
		token = strtok(NULL, ":");
		i= i+1;
	}//[0]=hours, [1]=minutes, [2]=seconds

	//Check validity of values
	//hours
	if(parArr[0] < 1 || parArr[0] > 24){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectHours, &falseHoursSize);
		return;
	}

	//minutes
	if(parArr[1] < 0 || parArr[1] > 59){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectMinutes, &falseMinutesSize);
		return;
	}
	//seconds
	if(parArr[2] < 0 || parArr[2] > 59){
		sys_req(WRITE, DEFAULT_DEVICE, incorrectSeconds, &falseSecondsSize);
		return;
	}
	
	//Reset Alarm Slots & Start the Alarm Process
	if(totalAlarms==0){
		load(alarmPcbName, &alarmProcess);//Start the alarm process if it hasn't already started
		//Empty Alarm List
		int j, k;
		for(k = 0; k < 100; k++){
			for(j = 0; j < 100; j++){
				alarmList[k][j]= '\0';
				alarmMessages[k][j]='\0';
			}
		}
	}
	//Fill Alarm
	while(alarmList[nextIndex]!=NULL){nextIndex++;}//Index of the the next empty slot for an alarm
	strcpy(alarmList[nextIndex], timeStatement);
	strcpy(alarmMessages[nextIndex], message);
	totalAlarms++;
	sys_req(WRITE, DEFAULT_DEVICE, success, &successSize);//Success
}

void alarmProcess(){
	//Error & Success Messages	
	char incorrectHours[100] = "\x1B[31mSome junk in the alarms list. I cleared it for you.\x1B[37m\n";
	char incorrectMinutes[100] = "\x1B[31mSome junk in the alarms list. I cleared it for you.\x1B[37m\n";
	char incorrectSeconds[100] = "\x1B[31mSome junk in the alarms list. I cleared it for you.\x1B[37m\n";
	int falseHoursSize = strlen(incorrectHours);
	int falseMinutesSize = strlen(incorrectMinutes);
	int falseSecondsSize = strlen(incorrectSeconds);	
	//Set Variables
	int i;
	char thisAlarm[100];
	int currentHour;
	int currentMinute;
	int currentSecond;
	char message[200];
	int parrArr[3];
	
	//check alarms
	for(i=0; i<possibleAlarms; i++){
		if(alarmList[i]!=NULL){
			//Alarm Message
			strcpy(message, "Alarm Completed: ");
			strcpy(message, alarmMessages[i]);
			strcpy(message, "\n ");
			int messageSize = strlen(message);
			
			//Divide time statement into components
			char* token = strtok(alarmList[i], ":");
			while (token != NULL){
				parArr[i] = atoi(token);
				token = strtok(NULL, ":");
				i= i+1;
			}//[0]=hours, [1]=minutes, [2]=seconds
			
			//Check validity of values
			//hours
			if(parArr[0] < 1 || parArr[0] > 24){
				sys_req(WRITE, DEFAULT_DEVICE, incorrectHours, &falseHoursSize);
				strcpy(alarmList[i], "");//Clean the slot
				totalAlarms--;
			}
			//minutes
			if(parArr[1] < 0 || parArr[1] > 59){
				sys_req(WRITE, DEFAULT_DEVICE, incorrectMinutes, &falseMinutesSize);
				strcpy(alarmList[i], "");//Clean the slot
				totalAlarms--;
			}
			//seconds
			if(parArr[2] < 0 || parArr[2] > 59){
				sys_req(WRITE, DEFAULT_DEVICE, incorrectSeconds, &falseSecondsSize);
				strcpy(alarmList[i],"");//Clean the slot
				totalAlarms--;
			}
			
			//Get Current Time
			outb(0x70, 0x04);
			currentHour = inb(0x71);
			outb(0x70, 0x02);
			currentMinute = inb(0x71);	
			outb(0x70, 0x00);
			currentSecond = inb(0x71);

			//Are we there yet?
			if(parArr[0]<currentHour){//Display Message:
				sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
				alarmList[i] = '\0';
				totalAlarms--;
			} else if(parArray[0]==currentHour){
				if(parArray[1]<currentMinute){//Display Message:
					sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
					alarmList[i] = '\0';
					totalAlarms--;
				} else if(parArray[1]==currentMinute){
					if(parArray[2]<=currentSecond){//Display Message:
						sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
						alarmList[i] = '\0';
						totalAlarms--;
					}
				}
			}
		}
	}
	if(totalAlarms==0){
		sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);//Oop you're out of alarms! Time to die
	} else {
		sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);//Every alarm has been checked. Idle now.
	}

}

void infinite(){

}
