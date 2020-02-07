
#include "polling.h"






int poll(char * buffer, int * count){
	int position=0;
	int totalChars=0;
	int flag=1;
	char letter;
	char sublet;
	int i=0;
	char tempLetAr[2]={'\0','\0'};
	int doPrint;
	//char temp[1]={0};
	
	while(flag){
		if (inb(COM1+5)&1){
			letter=inb(COM1);
			tempLetAr[0]=letter;
			doPrint=1;
			switch (letter){
			case 91:
				sublet=inb(COM1);
				doPrint=0;
				
				if (sublet==51){
					doPrint=0;
					serial_print("\[K");
					if (position<totalChars){
						for (i=position+1;i<totalChars;i++){
						
						tempLetAr[0]=buffer[i];
						serial_print(tempLetAr);
						buffer[i-1]=buffer[i];

						}
						buffer[i-1]='\0';
						for (i=position+1;i<totalChars;i++){
						
						tempLetAr[0]='\b';
						serial_print(tempLetAr);

						}
						totalChars=totalChars-1;
						//serial_print("\[K");	
					}
					sublet=inb(COM1);	
					
				}
				else if(sublet==67&&(position < *count)){
					doPrint=0;
					if(position==totalChars){
						buffer[position]=' ';
						totalChars=totalChars+1;

					}
					
					position=position+1;
					serial_print("\[1C");
				}
				else if(sublet==68){
					doPrint=0;
					if (position > 0){
						position=position-1;
					}
					serial_print("\[1D");

				}
				/*else{
					serial_print("HEYHELLO");
				}*/
				break;
			case 127:
				doPrint=0;
				if (position>0){
					position=position-1;
					tempLetAr[0]='\b';
					serial_print(tempLetAr);
					tempLetAr[0]=27;
					serial_print(tempLetAr);
					serial_print("\[K");
				
					if (position<totalChars){
						for (i=position+1;i<totalChars;i++){
					
						tempLetAr[0]=buffer[i];
						serial_print(tempLetAr);
						buffer[i-1]=buffer[i];

						}
						buffer[i-1]='\0';
						for (i=position+1;i<totalChars;i++){
					
						tempLetAr[0]='\b';
						serial_print(tempLetAr);

						}
					}
					if (totalChars>0){
						totalChars=totalChars-1;
					}
				}
				

				break;
			case 13:
				doPrint=0;
				//serial_print("hello there enter presser");

				serial_print("\nsending command: \n");
				serial_print(buffer);
				serial_print("\ntotal Chars: \n");
				tempLetAr[0]=totalChars+'0';
				serial_print(tempLetAr);
				serial_print(" \n");
				*count=totalChars;
				return 1;

				break;
			default:
				if (position < *count && letter>31 && letter<127){

					buffer[position]=letter;
					if (position==totalChars){
						totalChars=totalChars+1;

					}
					position=position+1;
				}
				
			

			}
			if ((totalChars>=0)&&(position < *count)&&(doPrint==1)){
				/*for(i=0;(i<totalChars+5)&&(i<*count);i++){
					serial_print(" ");
				}
				serial_print("   \r");
				serial_print(strcat(buffer,"\r"));*/
				//i=i+1;
				//serial_print("hi how are you");
				serial_print(tempLetAr);
			}
			
			//temp[0]=letter;
			//serial_print(temp);
			
			//serial_print(buffer);
		
		}

	}
	return 1;



}
