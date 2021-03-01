
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This header contains all the descriptions to each */

//passed the string descriptions back to main program 



 char *description_list(char *cur_itemname)
  { 
	char const *label[] = {
					 "INTEREST CAL", //0
					 "Fahren to Celc", //1
					 "Celc to Fahren", //2
					};
	int i;
	int n_label = 3;
	char *calc_descrip;

	for(i=0;i<n_label;i++){				
					
		if(strcmp(label[i],cur_itemname) == 0) //INTEREST CAL
		{
			sprintf(calc_descrip ,"This calculator computes interest of loan and produce monthly payments");
		   	return calc_descrip;
		}
		else if(strcmp(label[i],cur_itemname) == 0) //Fahren to Celc
		{	
			sprintf(calc_descrip ,"This calculator computes Fahrenheit to Celsius");
		   	return calc_descrip;
		}	
		//else if(strcmp(label[i],cur_itemname) == 0) //Celc to Fahren
		else
		{	
			sprintf(calc_descrip ,"This calculator computes Celsius to Fahrenheit");
		   	return calc_descrip;

		}	
	 
	} 
	
	
   }	

 
		
	
	
