
#include "polling.h"






int poll(char * buffer, int * count){
	int position=0;
	int totalChars=0;
	int flag=1;
	char letter;
	char sublet;
	//i=42;
	char tempLetAr[2]={'\0','\0'};
	int doPrint;
	//char temp[1]={0};
	//buffer[0]='\r';
	
	while(flag){
		if (inb(COM1+5)&1){
			letter=inb(COM1);
			tempLetAr[0]=letter;
			doPrint=1;
			switch (letter){
			case 91:
				sublet=inb(COM1);
				
				if (sublet==51){
					doPrint=0;
					//tempLetAr[0]='%';
					//serial_print(tempLetAr);
					if (position<totalChars){
						
						serial_print("\[1C");
						tempLetAr[0]='\b';
						serial_print(tempLetAr);
						//tempLetAr[0]='%';
						//serial_print(tempLetAr);	
					}	
					
				}
				else if(sublet==67&&position< *count){
					doPrint=0;
					if(position==totalChars){
						buffer[position]=' ';
						totalChars=totalChars+1;

					}
					
					position=position+1;
					serial_print("\[1C");
				}
				else if(sublet==68&&position>0){
					doPrint=0;
					position=position-1;
					serial_print("\[1D");

				}
				/*else{
					serial_print("HEYHELLO");
				}*/
				break;
			case 127:
				if (position>0){
					position=position-1;
					buffer[position]='\0';
					totalChars=totalChars-1;
					tempLetAr[0]='\b';

				}
				else if (position==0){
					buffer[position]='\0';
					tempLetAr[0]='\b';

				}
				break;
			default:
				if (position < *count ){

					buffer[position]=letter;
					position=position+1;
					totalChars=totalChars+1;
				}
				
			

			}
			if (totalChars>0&&position < *count&&doPrint){
				/*for(i=0;(i<totalChars+5)&&(i<*count);i++){
					serial_print(" ");
				}
				serial_print("   \r");

				
				serial_print(strcat(buffer,"\r"));*/
				//i=i+1;
				serial_print(tempLetAr);
				if (tempLetAr[0]=='\b'){
					tempLetAr[0]=' ';
					serial_print(tempLetAr);
					tempLetAr[0]='\b';
					serial_print(tempLetAr);
				}
			}
			else if (totalChars==0&&doPrint){
				/*for(i=0;i<totalChars+5;i++){
					serial_print(" ");
				}
				serial_print("   \r");*/
				serial_print(tempLetAr);
				if (tempLetAr[0]=='\b'){
					tempLetAr[0]=' ';
					serial_print(tempLetAr);
					tempLetAr[0]='\b';
					serial_print(tempLetAr);
				}
			}
			
			//temp[0]=letter;
			//serial_print(temp);
			
			//serial_print(buffer);
		
		}

	}
	return 1;



}
