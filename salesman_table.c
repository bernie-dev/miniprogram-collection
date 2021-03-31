/*Febru.01, 2021: Salesman TAble function needs a new separate program        */
/*Febru.08, 2021: modified number collection function (upgraded version)      */
/*March 15, 2021: Modified windows to become pads 							  */
/*March 16, 2021: Modified max character for InputIntegral for salesman_Table */
/*March 18, 2021: Create Fucnction for getting input for how many the salesman */
/*March 23, 2021: Added function getlistnames(),getname() for name collection   */ 
/*March 24, 2021: Added delCh() function										*/		
/*March 25, 2021: getname(), delCh,getlistnames(): added/modified PADREF pointers */	
/*March 26, 2021: Added trigger rows to scroll page nth times when near bottom of pad */
/*March 28, 2021: make row variable in getlistnames() a pointer variable			 */		
/*MArch 29, 2021: Change getSalesmanQtyF to getQtyF()								*/		
/*MArch 29, 2021: Added *message in getnQtyF()										*/	
/*March 29, 2021: Added scrbot_checker() function, 									 */		


#include "main_with_form.h"
#define DIGITSIZE 3
#define MAXINPUT 8
#define MAXCHARNAME 9
#define MAXINPUTNAME 20

int getQtyF(WINDOW *localwin, int row, int col, PAD_PRESH *padref, char *message); 
void delCh(WINDOW* local_win, int* delrow, int* delcol, int* charcount, int *strindex, int delboundary, PAD_PRESH *padref);
int getname(WINDOW *local_win, char *strname, int maxchar, int boundary, int *delrow, int *delcol, PAD_PRESH *padref);
char **getlistnames(WINDOW *local_win, int *row, int col, int numofmen, PAD_PRESH *padref);
void SalesmanErrorMessage(WINDOW *local_win, int row, int col, char *message, PAD_PRESH *padref);
void scrbot_checker(int *row, PAD_PRESH *padref);

//GLOBAL VARIABLES
int triggerline, oneline=1, triggerincrement; //for scrolling the pad whenever near bottom screen


void salesman_table(WINDOW *local_win, int ymax, int xmax)
{
   //set locale for wide characters
   setlocale(LC_ALL, ""); 
   
	int totalSman, totalcpnyname, subHeight=6, subWidth=xmax-2;
	int lwinrow,lwincol;
	//char IntBuf[DIGITSIZE];
	
	
	WINDOW* subpad1;
	PAD_PRESH padref; //needed by getlistnames, getname, delCh
	padref = get_prefresh();
	char message[80];
	char **listnames; 
	//int ch;
	
	//create values for triggerline
	int bottomscr = padref.HEIGHT-3; //output is 18
	triggerline = bottomscr- oneline; //output is 17
	triggerincrement =triggerline; //triggerincrement is 17;
	
   /*START########## Create a Subpad ######################START*/
    subpad1=subpad(local_win,subHeight,subWidth,1,1);    							
    if(subpad1==NULL)									
  		bomb("Unable to create subpad");	
  																	
    keypad(subpad1,TRUE);								
    wbkgd(subpad1,COLOR_PAIR(9)); 
   /*END########### Create a Subpad #########################END*/
	

  /*START:Instruction inside the window; Beginning of the program*/		
	wattron(subpad1,COLOR_PAIR(16) |A_BOLD);
	mvwprintw(subpad1, 0, 0, "This is salesman Table: Output will be the final commission received by each salesman\
based on the products they've selled. (1)Input no. of salesman, input names of salesman base on how many. (2) \
Input how many products, input names of each products. (3)Input how many salesman sells for each product. \n");
	wattroff(subpad1,COLOR_PAIR(16) | A_BOLD);
  /*END:Instruction inside the window; Beginning of the program*/		
   
    lwinrow=8;lwincol=1;   								
    totalSman=getQtyF(local_win,lwinrow,lwincol,&padref,"Enter number of salesman:");
    while(totalSman<=0){
		sprintf(message,"Error: zero value not accepted. Exiting...");
		errorMessage("Input zero not accepted");
		//curs_set(0);
		touchwin(subpad1);
		wrefresh(local_win);
		totalSman=getQtyF(local_win,lwinrow,lwincol, &padref,"Enter number of salesman:");
	}	
	//for the salesman count
    mvwprintw(local_win,++lwinrow,lwincol,"Number of Salesman: %dtriggerline=%d,bottomscr=%d", totalSman,triggerline,bottomscr);
    prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
    refresh();
    
    //for the salesmen names
    lwinrow+=2;
    mvwprintw(local_win,lwinrow++,lwincol, "Enter Name ID of Salesman (e.g. bernsagax)");
    prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
    listnames=getlistnames(local_win, &lwinrow, lwincol, totalSman, &padref);
    refresh();
    //for company names
    lwinrow++;
    scrbot_checker(&lwinrow, &padref);
    totalcpnyname=getQtyF(local_win, lwinrow,lwincol, &padref, "Enter number of Company:");
    while(totalcpnyname<=0){
		sprintf(message,"Error: zero value not accepted. Exiting...");
		errorMessage("Input zero not accepted");
		//curs_set(0);
		touchwin(subpad1);
		wrefresh(local_win);
		totalcpnyname=getQtyF(local_win,lwinrow,lwincol, &padref,"Enter number of Company:");
	}	
    mvwprintw(local_win,++lwinrow,lwincol,"Number of Company: %d", totalcpnyname);
    scrbot_checker(&lwinrow, &padref);
    prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
    refresh();
    
    

    wrefresh(local_win);
	refresh();
     
     //delwin(subpad1); 
	//delwin(local_win);
	
	return;
} 

int getQtyF(WINDOW *local_win, int row, int col, PAD_PRESH *padref, char *message)
{
  int total;
  
  mvwprintw(local_win,row,col,"%s", message);
  prefresh(local_win,padref->padystart,padref->padxstart,	padref->screenystart,padref->screenxstart,	padref->HEIGHT,padref->WIDTH);
  total=inputIntegral(local_win,1,padref);
  return total;
  
}	  

void delCh(WINDOW* local_win, int* delrow, int* delcol, int* charcount, int *strindex, int delboundary, PAD_PRESH *padref){
		//PAD_PRESH padref;
		//padref = get_prefresh();
		
		//Check if backspacing is out of bounds to input field	
			if(*delcol<=delboundary){
				*delcol=delboundary;
				wmove(local_win,*delrow,delboundary); //set 
				prefresh(local_win,padref->padystart,padref->padxstart,	padref->screenystart,padref->screenxstart,	padref->HEIGHT,padref->WIDTH);
				
			}
			else{
				if(*strindex>=0){
					wprintw(local_win,"\b \b");
					(*delcol)--;
					*charcount>0?(--(*charcount)):(*charcount=0);	
					--(*strindex);
									
					prefresh(local_win,padref->padystart,padref->padxstart,	padref->screenystart,padref->screenxstart,	padref->HEIGHT,padref->WIDTH);	
				}
				else
					*strindex=0;	
			}					
}		



int getname(WINDOW *local_win, char *strname, int maxchar, int delboundary, int *delrow, int *delcol, PAD_PRESH *padref){
	int c, charcount=0, strindex=0;
	//char *startptr = strname;
	while((c=wgetch(local_win))!='\n' && c != EOF){
		
	    //check character input if reach limit
		if(charcount>(maxchar-1)){
			if(c == 127 || c ==KEY_BACKSPACE)
				delCh(local_win, delrow, delcol, &charcount, &strindex, delboundary, padref);
			else	
				continue;
		}    

        //skip non-alphabetic characters except backspace(127,8,KEY_BACKSPACE)					
		if(!isalpha(c) && isspace(c) && (c!=127) && (c!=8) && c!=KEY_BACKSPACE)
			continue;													//skip it; go to the next alteration of the loop
		else if(isalpha(c)){

			*(strname+(strindex++)) = c; 								//same as strname[strindex++]=c
			wprintw(local_win,"%c",c);
			++charcount;
			(*delcol)++;
			prefresh(local_win,padref->padystart,padref->padxstart,	padref->screenystart,padref->screenxstart,	padref->HEIGHT,padref->WIDTH);
		}
		else if(c==KEY_BACKSPACE || c==127){
			delCh(local_win, delrow, delcol, &charcount, &strindex, delboundary, padref);
		}	
	}	 
	if(c == '\n')
		 *(strname+strindex)='\0';	 
	return charcount;	
		
	
}

char **getlistnames(WINDOW *local_win, int *row, int col,int numOfmen, PAD_PRESH *padref) {

//for ordinals
 char ordinals[] = {'s','t','n','d','r','d','t','h'};
 char strname[MAXCHARNAME];
 int j,x,ordvalue,lenname;
 int delrow, delcol;
 int dellef_bndry;
 

 keypad(local_win, TRUE);
 char **listnames=(char **)malloc(numOfmen * sizeof(char *)); //memory set aside
 
 
 for(j=0,x=1;j<numOfmen;j++,x++)
 {
	 //start-scroll function in pad
	 /*if(*row>=triggerline){
		padref->padystart = triggerline;
		triggerline+=triggerincrement;
	 }*/	
	 scrbot_checker(row, padref);
	 //end-scroll function in pad
	 
	 ordvalue=x<4? 2*(x-1):6;
	 mvwprintw(local_win, (*row)++, col, "Enter %d%c%c name: ",x, ordinals[ordvalue],ordinals[ordvalue+1]); //for prompt
	 getyx(local_win, delrow, delcol);
     dellef_bndry=delcol;
	 prefresh(local_win,padref->padystart,padref->padxstart,	padref->screenystart,padref->screenxstart,	padref->HEIGHT,padref->WIDTH);
	 
	 if((lenname=getname(local_win, strname, MAXCHARNAME, dellef_bndry, &delrow, &delcol, padref))>1) // this means name had atleast one character
	 {  
		char *str = malloc((len+1)*sizeof(char)); 
		strcpy(str,strname);  
		listnames[j] = str;
		mvwprintw(local_win, *row, col,"%d %d row=%d padrefh=%d %s",j,lenname,*row,padref->HEIGHT,listnames[j]);
		(*row)++;
		prefresh(local_win,padref->padystart,padref->padxstart,	padref->screenystart,padref->screenxstart,	padref->HEIGHT,padref->WIDTH);
		
	 }	
	 else
	   SalesmanErrorMessage(local_win, *row, col, "Input zero not accepted", padref); 
	  

 }	

 
    return listnames;

}

void scrbot_checker(int *row, PAD_PRESH *padref){
	if(*row>=triggerline){
		padref->padystart=triggerline;
		triggerline+=triggerincrement;
	}
}

void SalesmanErrorMessage(WINDOW *local_win, int row, int col, char *message, PAD_PRESH *padref){
  mvwprintw(local_win,row++,col,"%s\n",message);  
  prefresh(local_win,padref->padystart,padref->padxstart,	padref->screenystart,padref->screenxstart,	padref->HEIGHT,padref->WIDTH);
  
  exit(1);
}		


