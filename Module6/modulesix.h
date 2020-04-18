#ifndef MODULESIX_H_
#define MODULESIX_H_

#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

void initializeBootSector();
void printBootSector();
int getInt(int numBytes);
int bcdToInt(int numBytes, unsigned char* bytes);


#endif
