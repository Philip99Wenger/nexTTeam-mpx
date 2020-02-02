
#include "polling.h"






int poll(char * buffer, int * count){
	int position=0;
	int flag=1;
	char letter;
	//char temp[1]={0};
	*count=0;
	
	while(flag){
		if (inb(COM1+5)&1){
			letter=inb(COM1);
			buffer[position]=letter;
			position=position+1;
			//temp[0]=letter;
			//serial_print(temp);
			serial_print(strcat("\r",buffer));
		
		}

	}
	return 1;



}
