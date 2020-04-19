#ifndef MODULESIX_H_
#define MODULESIX_H_

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct bootSector{
	int bytesPerSector;
	int sectorsPerCluster;
	int numOfReservedSectors;
	int numOfFatCopies;
	int maxNumOfRootDirectoryEntries;
	int totalNumOfSectorsInFileSystem;
	int numOfSectorsPerFat;
	int sectorsPerTrack;
	int numOfHeads;
	int totalSectorCountForFat32;
	int bootSignature;
	int volumeID;
	char volumeLabel[12];
	char fileSystemType[9];

	
} bootSector;

typedef struct time{
	int hour;
	int minute;
	int second;
} time;

typedef struct date{
	int year;
	int month;
	int day;
} date;

typedef struct directory{
	char fileName[9];
	char extension[4];
	int attribute;
	int reserved;
	time creationTime;
	date creationDate;
	date lastAccessDate;
	time lastWriteTime;
	date lastWriteDate;
	int firstCluster;
	int fileSize;
} directory;

void initializeBootSector();
void initializeFatTable();
void printBootSector();
int getInt(int numBytes);
int bcdToInt(int numBytes, unsigned char* bytes);
void printDirectoryEntry(directory* current, int num);
void printOneFile(directory curr);
void setupDirectory(directory* dir, int startSec, int numEntries);
time getTime();
date getDate();
void printRootDirectory();
void changeDirectory(char* directoryName);
char * intToAscii(int integer);


#endif
