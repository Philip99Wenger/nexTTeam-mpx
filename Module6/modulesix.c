#include "modulesix.h"

FILE* filePointer;
bootSector boot;
int fatTable[2880]; //2880 = 512*9*5/8
directory root[224]; //14*16 = 224
directory* currentDir;
int sizeOfCurrentDir;
int startOfCurrentDir;
int quit;

int main(int argc, char *argv[])
{

	if(argc < 2){
		printf("The file name needs to be the first argument.\n");
		return -1;
	}

	filePointer = fopen(argv[1], "r+");

	initializeBootSector();
	initializeFatTable();

	//setup the root directory
	int startSec = 19;
	int i, j;
	for(i=0; i<14; i++){ //root directory goes from 19-32
		fseek(filePointer, 512*(startSec+i), SEEK_SET);
		for(j=0; j<16; j++){
			fread(root[16*i+j].fileName, 1, 8, filePointer);
			fread(root[16*i+j].extension, 1, 3, filePointer);
			root[16*i+j].attribute=getInt(1);
			root[16*i+j].reserved = getInt(2);
			root[16*i+j].creationTime = getTime();
			root[16*i+j].creationDate = getDate();
			root[16*i+j].lastAccessDate = getDate();
			fseek(filePointer, 2, SEEK_CUR);
			root[16*i+j].lastWriteTime = getTime();
			root[16*i+j].lastWriteDate = getDate();
			root[16*i+j].firstCluster = getInt(2);
			root[16*i+j].fileSize = getInt(4);
		}
	}

	currentDir = root;
	sizeOfCurrentDir = 224;
	startOfCurrentDir = 14;	

	if(!filePointer){
		printf("File was not found.\n");
		return -1;
	}

	unsigned int k;
	quit = 0;
	void (*printBoot_ptr)() = &printBootSector;
	void (*printRootDirectory_ptr)() = &printRootDirectory;
	void (*quitNow_ptr)() = &quitNow;
	//void (*gettime_ptr)() = &gettime;
	//void (*settime_ptr)() = &settimeWrapper;
	//void (*getdate_ptr)() = &getdate;
	//void (*setdate_ptr)() = &setdateWrapper;
	
	char commands[3][25]={
		"quit",
		"printBootSector", 
		"printRootDirectory",
		//"help",
		//"getTime",
		//"setTime",
		
	};
	void (*commands_ptrs[])()={
		*quitNow_ptr,
		*printBoot_ptr,
		*printRootDirectory_ptr,
		//*gettime_ptr,
		//*settime_ptr,
		//*getdate_ptr,
		
	};
	
	//Print welcome message
	char welcome[] = "Welcome to NextTeam's File Management!\nPlease type one of the available commands:\nquit\nprintBootSector\nprintRootDirectory\n\n";
	printf("%s", welcome);	
		
	//for(k=0; k<sizeof(commands); k++){
	//	printf("%s", commands[k]);
	//	printf("\n");
	//}	

	
	while(!quit){
		//get a command
		char newCommands[25];
		memset(newCommands, '\0', 25);
		scanf("%s", newCommands);
		char* cmdBuffer = strtok(newCommands, " ");
		//pass

		//
		char noCommand[] = "There is no matching command. Commands are case-sensitive.\n";

		int matchFlag = 0;
		for(k=0; k<sizeof(commands); k++){
			if(strcmp(cmdBuffer, commands[k])==0){
				(*commands_ptrs[k])();
				matchFlag = 1;
			}
		}
		if(matchFlag == 0){printf("%s", noCommand);}
	}
	
}

void initializeBootSector(){
	fseek(filePointer, 11, SEEK_SET);
	boot.bytesPerSector = getInt(2);
	boot.sectorsPerCluster = getInt(1);
	boot.numOfReservedSectors = getInt(2);
	boot.numOfFatCopies = getInt(1);
	boot.maxNumOfRootDirectoryEntries = getInt(2);
	boot.totalNumOfSectorsInFileSystem = getInt(2);
	fseek(filePointer, 22, SEEK_SET);
	boot.numOfSectorsPerFat = getInt(2);
	boot.sectorsPerTrack = getInt(2);
	boot.numOfHeads = getInt(2);
	fseek(filePointer, 32, SEEK_SET);
	boot.totalSectorCountForFat32 = getInt(4);
	fseek(filePointer, 38, SEEK_SET);
	boot.bootSignature = getInt(1);
	boot.volumeID = getInt(4);
	fread(boot.volumeLabel, 1, 11, filePointer);
	fread(boot.fileSystemType, 1, 8, filePointer);
}

void initializeFatTable(){
	int i;
	int sector = 1;
	fseek(filePointer, 512*sector, SEEK_SET);
	for (i=0; i<2880; i=i+2){
		unsigned char current[3];
		fread(current, 1, 3, filePointer);
		fatTable[i] = ((current[1] & 0x0f) << 8) + current[0];
		fatTable[i+1] = ((current[2] & 0xf0) << 4) + ((current[2] & 0x0f) << 4) + ((current[1] & 0x0f) >> 4);
	}
	
}
void printBootSector(){
	printf("Bytes per Sector: %d\n", boot.bytesPerSector);
	printf("Sectors per Cluster: %d\n", boot.sectorsPerCluster);
	printf("Number of Reserved Sectors: %d\n", boot.numOfReservedSectors);
	printf("Number of FAT Copies: %d\n", boot.numOfFatCopies);
	printf("Max Number of Root Directory Entries: %d\n", boot.maxNumOfRootDirectoryEntries);
	printf("Total Number of Sectors in the File System: %d\n", boot.totalNumOfSectorsInFileSystem);
	printf("Number of Sectors per FAT: %d\n", boot.numOfSectorsPerFat);
	printf("Sectors per Track: %d\n", boot.sectorsPerTrack);
	printf("Number of Heads: %d\n", boot.numOfHeads);
	printf("Total Sector Count for FAT32: %d\n", boot.totalSectorCountForFat32);
	printf("Boot Signature: %d\n", boot.bootSignature);
	printf("Volume ID: %d\n", boot.volumeID);
	printf("Volume Label: %s\n", boot.volumeLabel);
	printf("File System Type: %s\n\n", boot.fileSystemType);

}

int getInt(int numBytes){
	unsigned char current[numBytes];
	fread(current, 1, numBytes, filePointer);
	return bcdToInt(numBytes, current);
}

int bcdToInt(int numBytes, unsigned char* bytes){
	int i;
	int integerValue = 0;
	for(i=0; i<numBytes; i++){
		integerValue = integerValue + ((bytes[i] & 0xff) << (8*i));
	}
	return integerValue;
}

void printDirectoryEntry(directory* current, int num){
	int i;
	for(i=0; i<num; i++){
		if((unsigned char) current[i].fileName[0] == 0x00) //remaining files are free 
			break;
		else if((unsigned char) current[i].fileName[0] == 0xE5) //file is empty
			continue;
		else if (((unsigned int) current[i].attribute & 0x02) == 0x02) //file is hidden
			continue;
		else
			printOneFile(current[i]);
		
	}
}

void printOneFile(directory curr){
	printf("File Name: %s\n", curr.fileName);
	printf("Extension: %s\n", curr.extension);
	printf("Attribute: %d\n", curr.attribute);
	printf("Reserved: %d\n", curr.reserved);
	printf("Creation Time: %s:%s:%s\n", intToAscii(curr.creationTime.hour), intToAscii(curr.creationTime.minute), intToAscii(curr.creationTime.second));
	printf("Creation Date: %d/%d/%d\n", curr.creationDate.month, curr.creationDate.day, curr.creationDate.year);
	printf("Last Access Date: %d/%d/%d\n", curr.lastAccessDate.month, curr.lastAccessDate.day, curr.lastAccessDate.year);
	printf("Last Write Time: %s:%s:%s\n", intToAscii(curr.lastWriteTime.hour), intToAscii(curr.lastWriteTime.minute), intToAscii(curr.lastWriteTime.second));
	printf("Last Write Date: %d/%d/%d\n", curr.lastWriteDate.month, curr.lastWriteDate.day, curr.lastWriteDate.year);
	printf("First Cluster: %d\n", curr.firstCluster);
	printf("File Size: %d\n\n", curr.fileSize);
}

//set up directory can be used for changing the directory
void setupDirectory(directory* dir, int startSec, int numEntries){
	int i;
	int currSector;
	fseek(filePointer, 512*(31+startSec), SEEK_SET);
	for (int i=0; i<numEntries; i++){
		if(i != 0 && i%512 == 0){
			currSector = fatTable[currSector];
			fseek(filePointer, 512*(31+startSec), SEEK_SET);
		}
		fread(dir[i].fileName, 1, 8, filePointer);
		fread(dir[i].extension, 1, 3, filePointer);
		dir[i].attribute=getInt(1);
		dir[i].reserved = getInt(2);
		dir[i].creationTime = getTime();
		dir[i].creationDate = getDate();
		dir[i].lastAccessDate = getDate();
		fseek(filePointer, 2, SEEK_CUR);
		dir[i].lastWriteTime = getTime();
		dir[i].lastWriteDate = getDate();
		dir[i].firstCluster = getInt(2);
		dir[i].fileSize = getInt(4);
	}
	
}

time getTime(){
	time t;
	unsigned char current[2];
	fread(current, 1, 2, filePointer);
	int hour = 0;
	hour = hour + ((current[1] & 0xf8) >> 3); //hour is bits 15-11
	t.hour = hour;
	int minute = 0;
	minute = minute + ((current[2] & 0xe0) >> 5) + ((current[1] & 0x07) << 3); //minute is bits 10-5
	t.minute = minute;
	int second = 00;
	second = second + (current[2] & 0x1f); //second is bits 4-0
	t.second = second;
	return t;
}

date getDate(){
	date d;
	unsigned char current[2];
	fread(current, 1, 2, filePointer);
	int year = 1980; //starts at year 1980
	year = year + ((current[1] & 0xfe) >> 1); //year is bits 15-9
	d.year = year;
	int month = 0;
	month = month + ((current[2] & 0x01) << 3) + ((current[1] & 0xe0) >> 5); //month is bits 8-5
	d.month = month;
	int day = 0;
	day = day + (current[2] & 0x1f); //day is bits 4-0
	d.day = day;
	return d;
}

void printRootDirectory(){
	printDirectoryEntry(root, 224);
}

void changeDirectory(char* directoryName) {
	if(directoryName ==  NULL){ //if null, reset to root
		if(currentDir != root) {
			free(currentDir);
		}
		int startSector = 19;
		fseek(filePointer, 512*(startSector), SEEK_SET);
		sizeOfCurrentDir = 224;
		startOfCurrentDir = 14;
	} 
	//THIS NEEDS TO BE COMPLETED
	else {
		int location = getDirectoryLocation(directoryName, "", 0);
		printf("Location %d\n", location);

		if(location != -1){
			int sectors = 9;
			int numEntries = sectors*16;

			directory *temp = malloc(sizeof(directory)*numEntries);
			int currSector = currentDir[location].firstCluster;
			fseek(filePointer, 512*(31+currSector), SEEK_SET);
			setupDirectory(temp, currSector, numEntries);
			startOfCurrentDir = currentDir[location].firstCluster;
			
			currentDir = temp;
			sizeOfCurrentDir = numEntries;
			}

		else
			printf("Did not find directory %s.\n\n", directoryName);
		
	}
}

int getDirectoryLocation(char* name, char* extension, int start){
	if(extension == "   " || extension == NULL){
		extension = "";
	}

	int j;

	for(j=start; j<sizeOfCurrentDir; j++){
		if(((unsigned char)currentDir[j].fileName[0])==0x00)
			break;
		else if(((unsigned char)currentDir[j].fileName[0])==0xE5)
			continue;
		else if(((strcmp(name, "*") == 0) || (strcmp((currentDir[j].fileName), name)==0)) && ((strcmp(extension, "*") == 0) || (strcmp((currentDir[j].fileName), name)==0)))
			return j;
	}
	return -1;
}

char * intToAscii(int integer){

           //Split the Digits
           int ones = integer%10;
           integer/=10;
           int tens = integer%10;
	   integer/=10;

           //Convert to Char 
  	  char OnesPlace = ones+'0';
           char TensPlace = tens+'0';

		//Place inside Array
        	char array[4] = {(char) TensPlace, (char) OnesPlace, '\0'};

           	//Point to Array
           	char *arrayPoint = array;

           	//Return the Array
     	   	return arrayPoint;
}

char * removeWhiteSpaces(char *word){
	char *temp = word;
	int i;	

	if (word == NULL){
		return NULL;
	}

	if(word[0] = '\0'){
		return word;
	}
	
	while(word[i] != '\0'){
		if(word[i] == ' '){
			temp[i] = temp[i+1];
			i++;
		}
	}

	return temp;
	
}

void quitNow(){
	quit=1;
}

