
#include "polling.h"






int poll(char * buffer, int * count){
	int position=0;
	int totalChars=0;
	int flag=1;
	char letter;
	int i;
	//char temp[1]={0};
	//buffer[0]='\r';
	
	while(flag){
		if (inb(COM1+5)&1){
			letter=inb(COM1);
			switch (letter){
			case '~':
				return 1;
				break;
			case 127:
				if (position>0){
					position=position-1;
					buffer[position]='\0';
					totalChars=totalChars-1;

				}
				else if (position==0){
					buffer[position]='\0';

				}
				break;
			default:
				if (position < *count ){

					buffer[position]=letter;
					position=position+1;
					totalChars=totalChars+1;
				}
				
			

			}
			if (totalChars>0){
				for(i=0;(i<totalChars+5)&&(i<*count);i++){
					serial_print(" ");
				}
				serial_print("   \r");
				serial_print(strcat(buffer,"\r"));
			}
			else if (totalChars==0){
				for(i=0;i<totalChars+5;i++){
					serial_print(" ");
				}
				serial_print("   \r");
			}
			
			//temp[0]=letter;
			//serial_print(temp);
			
			//serial_print(buffer);
		
		}

	}
	return 1;



}
