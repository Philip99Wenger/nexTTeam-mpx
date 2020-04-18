#include "modulesix.h"

FILE* filePointer;
bootSector boot;

int main(int argc, char *argv[])
{

	if(argc < 2){
		printf("The file name needs to be the first argument.\n");
		return -1;
	}

	filePointer = fopen(argv[1], "r+");

	initializeBootSector();

	if(!filePointer){
		printf("File was not found.\n");
		return -1;
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

