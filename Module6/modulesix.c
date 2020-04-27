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

	//initialize the boot sector and the fat table
	initializeBootSector();
	initializeFatTable();
	//changeDirectory(NULL);
	char fileName[] = "SUBDIR";
	changeDirectory(fileName);

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
	startOfCurrentDir = 19;	

	if(!filePointer){
		printf("File was not found.\n");
		return -1;
	}

	unsigned int k;
	quit = 0;
	//setup pointers
	void (*printBoot_ptr)() = &printBootSector;
	void (*printRootDirectory_ptr)() = &printRootDirectory;
	void (*changeDirectory_ptr)() = &changeDirectoryWrapper;
	void (*quitNow_ptr)() = &quitNow;
	void (*help_ptr)() = &help;
	void (*list_ptr)() = &listWrapper;
	void (*renameFile_pointer)() = &renameFile;
	void (*type_ptr)() = &typeWrapper;
	
	char commands[8][25]={
		"quit",
		"help",
		"printBootSector", 
		"printRootDirectory",
		"changeDirectory",
		//"help",
		//"getTime",
		//"setTime",
		"list",
		"rename", 
		"type"
		
	};
	void (*commands_ptrs[])()={
		*quitNow_ptr,
		*help_ptr,
		*printBoot_ptr,
		*printRootDirectory_ptr,
		*changeDirectory_ptr,
		//*gettime_ptr,
		//*settime_ptr,
		//*getdate_ptr,
		*list_ptr,
		*renameFile_pointer, 		
		*type_ptr
		
	};
	
	//Print welcome message
	char welcome[] = "Welcome to NextTeam's File Management!\nPlease type one of the available commands:\nquit\nhelp\nprintBootSector\nprintRootDirectory\nchangeDirectory\nlist\nrename\ntype\n\n";
	printf("%s", welcome);	
		
	//for(k=0; k<sizeof(commands); k++){
	//	printf("%s", commands[k]);
	//	printf("\n");
	//}	

	//while the user has not entered quit, keep getting user input
	while(!quit){
		//get a command
		char newCommands[25];
		memset(newCommands, '\0', 25);
		scanf(" %[^\n]", newCommands);
		char* cmdBuffer = strtok(newCommands, " ");
		//pass

		//
		char noCommand[] = "There is no matching command. Commands are case-sensitive.\n";

		int matchFlag = 0;
		//if there is a command match, then go to the correct command pointer
		for(k=0; k<sizeof(commands); k++){
			if(strcmp(cmdBuffer, commands[k])==0){
				(*commands_ptrs[k])();
				matchFlag = 1;
			}
		}
		if(matchFlag == 0){printf("%s", noCommand);}
	}
	
}

void help(){
	//display information for how to execute each command
	printf("quit\n\tEnds execution of module 6\n");	
	printf("printBootSector\n\tPrints out all information in the boot sector\n");
	printf("printRootDirectory\n\tPrint all the info in the root directory\n");
	printf("changeDirectory <directoryName>\n\tChanges the directory to a subdirectory of the current directory\n");
	printf("list <file.extension>\n\tLists the info for current directory by default, but lists for specified file if specified\n");
	printf("rename <oldFileName.extension> <newFileName.extension>\n\tChanges the file name\n");
	printf("type <fileName.extension>\n\tDisplay the file entered\n");
}

void initializeBootSector(){
	//get the values for each variable in the struct boot
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
	//read in the values from the fat table into a global variable
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
	//print a decription of each variable and print the variable value for boot
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
	//read in the number of specified bytes and return an integer
	unsigned char current[numBytes];
	fread(current, 1, numBytes, filePointer);
	return bcdToInt(numBytes, current);
}

int bcdToInt(int numBytes, unsigned char* bytes){
	//convert the unsigned char bytes to an integer
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
		else {//if the file is not free, empty, or hidden, display the file
			printOneFile(current[i]);
		}
		
	}
}

void printOneFile(directory curr){
	//print all of the attributes for on entry in the directory
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
	//read in the values for the directory entry for the number of entries
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
	//read in two bytes for the time and & with the correct bits to get hour, min, and second
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
	//read in two bytes for the date and & with the correct bits to get year, month, and day
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
	//go to printDirectoryEntry for the root
	printDirectoryEntry(root, 224);
}

void changeDirectoryWrapper(){
	//get the directory name entered by the user
	char* directoryN = strtok(NULL, " ");
	printf("%s\n", directoryN);
	char s1[25]; 
	strcpy(s1, directoryN);
	changeDirectory(s1);
	//changeDirectory(directoryN);
	//changeDirectory(directoryName);
}

void changeDirectory(char* directoryName) {
	//char* directoryName = strtok(NULL, " ");
	//printf("%s\n", directoryName);
	if(directoryName ==  NULL){ //if null, reset to root
		printf("Hi NULL here\n");
		int startSector = 19;
		currentDir = root;
		fseek(filePointer, 512*(startSector), SEEK_SET);
		sizeOfCurrentDir = 224;
		startOfCurrentDir = 14;
	} 
	//THIS NEEDS TO BE COMPLETED
	else if (directoryName != NULL){
		printf("Hello");
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
			printf("Successfully Changed Directory\nPrinting current directory...\n\n");
			printDirectoryEntry(currentDir, sizeOfCurrentDir);
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

	fseek(filePointer, 512*(startOfCurrentDir), SEEK_SET);
	//for the size of the directory, if the file is not free or deleted, check if the name and extension entered matches the entry and return if it matches
	for(j=start; j<sizeOfCurrentDir; j++){
		int compare = strncmp(currentDir[j].fileName, name, strlen(name));
		if(((unsigned char)currentDir[j].fileName[0])==0x00)
			break;
		else if(((unsigned char)currentDir[j].fileName[0])==0xE5)
			continue;
		else if(compare == 0)
			return j;
	}
	//int j=0;
	//while (currentDir[j].fileName[0] != 0x00){
	//	if (strcmp(currentDir[j].fileName, name) == 0 && strcmp(currentDir[j].extension, extension) == 0){
	//		return j;
	//	}
	//}	j++;
	return -1;
}

void listWrapper(){
	//get the file name and extension entered and then go to list the directory
	char* fileName;
	char* extension;
	char* theRest = strtok(NULL, "");
	if(theRest=="\0"){fileName = "\0"; extension = "\0";} else{
		fileName = strtok(theRest, ".");
		extension = strtok(NULL, "\0");
	}
	listDirectory(fileName, extension);
}

void listDirectory(char* fileName, char* extension){
	char* nullPtr = "\0";
	printf("\n%s\n", fileName);
	printf("\nDirectory Name: %s", (*currentDir).fileName);
	if((*currentDir).extension!=NULL){printf(".%s", (*currentDir).extension);}
	printf("\nDirectory Size: %d", (*currentDir).fileSize);

	//Properties
	if((*currentDir).attribute!=0x00){
		printf("\nThis Directory has the following properties:\n");
		int readOnly = (*currentDir).attribute & 0x01;
		int hidden = (*currentDir).attribute & 0x02;
		int system = (*currentDir).attribute & 0x04;
		int subdirectory = (*currentDir).attribute & 0x10;
		if(readOnly==0x01){printf("Read Only\n");}
		if(hidden==0x02){printf("Hidden\n");}
		if(system==0x04){printf("System\n");}
		if(subdirectory==0x10){printf("Is a subdirectory\n");}
		if((*currentDir).reserved!=0x00){printf("Reserved\n");}
	}
	
	//Time Information
	printf("Creation Time: %s:%s:%s\n", intToAscii((*currentDir).creationTime.hour), intToAscii((*currentDir).creationTime.minute), intToAscii((*currentDir).creationTime.second));
	printf("Creation Date: %d/%d/%d\n", (*currentDir).creationDate.month, (*currentDir).creationDate.day, (*currentDir).creationDate.year);
	printf("Last Access Date: %d/%d/%d\n", (*currentDir).lastAccessDate.month, (*currentDir).lastAccessDate.day, (*currentDir).lastAccessDate.year);
	printf("Last Write Time: %s:%s:%s\n", intToAscii((*currentDir).lastWriteTime.hour), intToAscii((*currentDir).lastWriteTime.minute), intToAscii((*currentDir).lastWriteTime.second));
	printf("Last Write Date: %d/%d/%d\n", (*currentDir).lastWriteDate.month, (*currentDir).lastWriteDate.day, (*currentDir).lastWriteDate.year);

	//Files & Directories
	printf("The Files and Folders Contained in this Directory:\n\n");
	int location = 0;
	if(fileName==NULL){//if the user just entered "list", print the whole current directory
		while((unsigned char) currentDir[location].fileName[0] != 0x00){location++;}
		printDirectoryEntry(currentDir, location);
	} else {
		while(location>=-1){
			location = getDirectoryLocation(fileName, extension, location);
			if(location>=-1){printDirectoryEntry(currentDir, location);}
		}
		

		/*unsigned char startCluster[2];	//temporary start-cluster holder
		unsigned char filename[8];	//temporary file-name holder
		unsigned char extension[3];	//temporary extension holder
		unsigned char size[4];		//temporary size holder
		unsigned char byteHolder[1];	//temporary byteHolder
		int sectorSize = 512;		//Size of a sector (fixed)
		int incrementSector = 0;	//how much of the sector have we been through
		int location = 0;		//where we read the data from
		int currentSector = (*currentDir).firstCluster;
		int t, s;
		while(currentSector!=0xFF8){					//Increment till end of file
			fseek(filePointer, sectorSize*currentSector, SEEK_SET);	//Start at the beginning of the sector
			while(incrementSector<=sectorSize){			//Increment till end of sector
				location = (sectorSize*currentSector)+incrementSector;
				fread(byteHolder, 1, 1, filePointer);//=1
				filename[7] = *byteHolder;
				printf("signal->>>  %d\n", filename[7]);
				if(filename[7]==0xE5){
					printf("Empty File\n");
				} else if(filename[7]==0x00){
					printf("All else is free\n");
					return;
				} else{					//Where The Magic Happens ->


					//Print Name
					t = 6;
					for(s=0; s<7; s++){
						fread(byteHolder, 1, 1, filePointer);//=8
						filename[t] = *byteHolder;
						t--;
					}
					printf("Name: %s", filename);
					//Print Extention
					t = 2;
					for(s=0; s<3; s++){
						fread(byteHolder, 1, 1, filePointer);//=11
						extension[t] = *byteHolder;
						t--;
					}
					printf(".%s", extension);
					fseek(filePointer, 16, SEEK_CUR);//=26
					t = 1;
					for(s=0; s<2; s++){
						fread(byteHolder, 1, 1, filePointer);//=28
						startCluster[t] = *byteHolder;
						t--;
					}
					//Print Size
					t = 3;
					for(s=0; s<4; s++){
						fread(byteHolder, 1, 1, filePointer);//=32
						size[t] = *byteHolder;
						t--;
					}
					printf("	Size: %d bytes", (int)*size);
					//Print Start Cluster
					printf("	Start Cluster: %d\n\n", (int)*startCluster);


				}					//Where The Magic Happens ^^^^^
				incrementSector = incrementSector + 32;	
				if(fatTable[currentSector+2]==0xFF8){break;}	//break if at the end of file before end of sector
			}
			currentSector = fatTable[currentSector+2];		//Find the next sector in the FAT
			if((currentSector==0x00)||(currentSector==0xFF7)){	//If that sector is broken or empty
				printf("\x1B[31mThere is an unfortunate error.\x1B[37m\n");
				return;
			}
			if(currentSector>=0xFF8){				//If this is true, it should be the end
				printf("  '->  End of Directory\n");
			}
			incrementSector = 0;					//Restart to the beginning of the sector
		}*/
	}
	
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
	size_t len=0;
	char *temp = word;
	char *end = NULL;
	int i;	

	if (word == NULL){
		return NULL;
	}

	if(word[0] = '\0'){
		return word;
	}
	
	len = strlen(word);
	end = word + len;

	//while the front is a space, remove the first character
	while(isspace(*temp)){++temp;}
	if( end != temp){
		//while the end is a space, move one character in
		while(isspace(*(--end)) && end != temp) {}
	}

	//set the null terminating character if temo, end, and word do not match
	if(word + len -1 != end)
		*(end+1) = '\0';
	else if(temp != word && end == word)
		*word = '\0';

	end = word;
	if(temp != word){
		while(temp){end == temp;}
		*end = '\0';
	}

	return word;
	
}

void quitNow(){
	//set quit to 1 so the program quits
	quit=1;
}

void renameFile(){
	char* oldFile = strtok(NULL, " ");
	char* newFile = strtok(NULL, " ");

	//print error message if two files were not entered
	if(!oldFile || !newFile){
		printf("Two Files are needed\n");
	}
	
	else{
		//get the old name and extension
		char* oldName = strtok(oldFile, ".");
		char* oldExtension = strtok(NULL, "");
		if(!oldExtension){
			oldExtension = "   ";
		}
		//get the location of the old file
		int location = getDirectoryLocation(oldName, oldExtension, 0);
		printf("Location %d\n", location);

		//get the new name and extension
		char* newName = strtok(newFile, ".");
		char* newExtension = strtok(NULL, "");
		if(!newExtension){
			newExtension = "   ";
		}
		//if there's no old name, new name, or location not found, print error
		if(!oldName || !newName || location==-1){
			printf("File name cannot be changed\n");
		}
		else if(newExtension != "   " && strcasecmp(removeWhiteSpaces(oldExtension), removeWhiteSpaces(newExtension)) != 0){
			printf("File extension cannot be changed\n");
		}
		else{

			int j;
			int sector = location/16;
			int locationWithinSector = location - 16*sector;

			//set the pointer if current directory is root
			if(currentDir ==  root){
				fseek(filePointer, 512*(19+sector), SEEK_SET);
			}	
			else{
				//set the pointer if not in root
				int currentSector = startOfCurrentDir;
				for(j=0; j<sector; j++){
					currentSector = fatTable[currentSector];
				}
				fseek(filePointer, 512*(31+currentSector), SEEK_SET);
			}

			//find the old file location and change the name and extension
			fseek(filePointer, 32*locationWithinSector, SEEK_CUR);
			fwrite(newName, 1, 8, filePointer);
			fwrite(newExtension, 1, 3, filePointer);

			//change the file name in the currentDir array
			for(j=0; j<8; j++){
				if(j<strlen(newName)){
					currentDir[location].fileName[j]=newName[j];
				}
				else{
					currentDir[location].fileName[j] = ' ';
				}
			}

			//change the file extension in currentDir array
			for(j=0; j<3; j++){
				if(j<strlen(newName)){
					currentDir[location].extension[j]=newExtension[j];
				}
				else{
					currentDir[location].fileName[j] = ' ';
				}
			}
			
			printf("File Name Successfully Changed\n");
			//set the pointer based off if the current directory is root
			if (currentDir ==  root){
				fseek(filePointer, 512*(14), SEEK_SET);
			}
			else{
				fseek(filePointer, 512*(31+startOfCurrentDir), SEEK_SET);
			}
		}
	}
}

void typeWrapper(){
	//get the file that should be displayed
	char* fileNameEntered = strtok(NULL, " ");

	//print error if no file name is included
	if(fileNameEntered == NULL){
		printf("File must be included\n");
	}

	//get the file name and the extension if file name was entered
	else{
		char* name = strtok(fileNameEntered, ".");
		char* extension = strtok(NULL, " ");

		type(name, extension);
	}
}

void type(char* name, char* extension){
	if(extension == NULL)
		extension = "";
	if((strncmp(extension, "BAT", 3)==0) || (strncmp(extension, "BAT", 3)==0) || (strncmp(extension, "C", 1)==0)){
		printf("Can only display files with extension of BAT, TXT, or C\n");
		return;
	}

	int location = getDirectoryLocation(name, extension, 0);

	if(location == -1){
		printf("The file entered was not found in the current directory\n");
		return;
	}

	int a=0;
	int b=0; 

	int currSector = currentDir[location].firstCluster;
	fseek(filePointer, 512*(currSector+31), SEEK_SET);

	char buffer[513];
	buffer[512] = '\0';

	int sector = numSectors(currSector);
	while(b < sector && a < currentDir[location].fileSize){
		if((currentDir[location].fileSize-a) > 512){
			fread(buffer, 1, 512, filePointer);
			printf("%s", buffer);
			a=a+512;
			getchar();
		}
		else{
			fread(buffer, 1, currentDir[location].fileSize-a, filePointer);
			buffer[currentDir[location].fileSize-a]='\0';
			printf("%s", buffer);
		}
		currSector = fatTable[currSector];
		fseek(filePointer, 512*(currSector+31), SEEK_SET);
		b++;
	}
	printf("Finished printing file\n");
	
}

int numSectors(int startSector){
	int k=1;

	while(1){
		//if the file is in the last cluster, the next cluster is unused, or the next cluster is bad, break
		if(((startSector >= 0xFF8) && (startSector <= 0xFFF) || (fatTable[startSector] == 0x00) || (fatTable[startSector] == 0xFF7))){
			break;
		}
		startSector = fatTable[startSector];
		k++;
	}
	return k;
}
