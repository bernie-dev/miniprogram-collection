/*Syntax Modification History: */
/*Bernard Saga: Dec 15, 2020: Fixed the exit function to not crushed the terminal screen  */
/*Bernard Saga: Dec 27, 2020: Transfer item_desc function to calcu_function source file   */
/*Bernard Saga: Feb 16, 2021: Fixed item_desc function's sigsev regarding wide characters */
/*Bernard Saga: Feb 20, 2021: Added bomb function										  */
/*Bernard Saga: Feb 28, 2021: Added navigation to interest_calc_win funuction             */
/*Bernard Saga: Mar 03, 2021: Added comment for git excercices						  	  */
/*Bernard Saga: Mar 04, 2021: Remove subpad prefreshing in interest_calc_win			  */ 


#include "main_with_form.h"
#define maxinput 8

void interest_calc_win(WINDOW *local_win, int maxrow, int maxcol ) //row 0, col 1;
{
  WINDOW* subpad1;	
  PAD_PRESH padref;
  //char charlocal_buff[len];
  float principal,rate=0.0,MonthlyRepay,TotalPayment;
  int row=2,col=1, numYrs=0, subHEIGHT, subWIDTH, navch, rowpadnav, rowcount =197;
  padref = get_prefresh();										//populate padref members
  subHEIGHT=6;
  subWIDTH=maxcol-2;
  //subWIDTH=52;
  
  
  /*START########## Create a Subpad ######################START*/
  /*#*/	subpad1=subpad(local_win,subHEIGHT,subWIDTH,1,1);    /*#*/							
  /*#*/	if(subpad1==NULL)									/*#*/
  /*#*/		bomb("Unable to create subpad");				/*#*/
  /*#*/														/*#*/
  /*#*/ keypad(subpad1,TRUE);								/*#*/
        //win_border(subpad1,0);
        wbkgd(subpad1,COLOR_PAIR(16)); 
  /*END########### Create a Subpad #########################END*/	 	
  mvwprintw(stdscr,1,1,"maxrow=%d, maxcol=%d",maxrow,maxcol); //maxrow=200, maxcol=54
  wattron(subpad1,COLOR_PAIR(16)| A_BOLD);
  mvwprintw(subpad1,row++,col,"Input numbers only, decimal point is accepted."); //supposed to be 1
  mvwprintw(subpad1,row++,col,"Sample Input: Principal:5000;Rate;15.5%%;Noofyears:5;monthly:125.77");
  wattroff(subpad1,COLOR_PAIR(16)| A_BOLD);
  refresh();
  row=8;
  /*START################# MAIN PROGRAM #############################START*/
  mvwprintw(local_win,row++,col,"Enter Principal:");
  prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);	
  principal=inputIntegral(local_win, stdscr, maxinput,padon);
  mvwprintw(local_win,row++,col,"%.2f",principal);
  mvwprintw(local_win,row,col,"Enter Rate(in %):");
  prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
  rate=inputIntegral(local_win, stdscr, maxinput,padon);
  row++;
  mvwprintw(local_win,row++,col,"%.2f%%",rate);
  mvwprintw(local_win,row,col,"Enter No. years:");
  prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
  numYrs=inputIntegral(local_win,local_win,2,padon);
  row++;
  mvwprintw(local_win,row++,col,"%d",numYrs);
  prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
  MonthlyRepay=compute(principal,rate,numYrs);
  TotalPayment=MonthlyRepay*(numYrs*12);
  row++;
  /*END ########################## MAIN PROGRAM ###############################END*/
  
  /*START################## FINAL OUTPUT ##################################START*/ 
  mvwprintw(local_win,row++,col,"Principal: %.2f", principal);  
  mvwprintw(local_win,row++,col,"Rate(%%): %.2f", rate);
  mvwprintw(local_win,row++,col,"Number of years: %d", numYrs);
  mvwprintw(local_win,row++,2,"Monthly Payments: %.2f", MonthlyRepay);
  mvwprintw(local_win,row++,2,"Total Payments: %.2f", TotalPayment);
  mvwprintw(local_win,row++,2,"Total Interest: %.2f", TotalPayment-principal);
  wattron(subpad1,COLOR_PAIR(11)|A_BOLD);
  mvwprintw(subpad1,1,1,"Navigation mode: press 'q' to quit, up and down keys to navigate.");
  wattroff(subpad1,COLOR_PAIR(11)|A_BOLD);
  prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
  
  /*END################### FINAL OUTPUT ###################################END*/
  
  doupdate();
  refresh();
  
    /*START############## NAVIGATION OF PROGRAM ##########################START*/
    rowpadnav = padref.padystart;
	while((navch=wgetch(local_win))!='q')
	{
		switch(navch)
		{
		 case KEY_UP:
			if(rowpadnav>=0){
				rowpadnav--;}
			prefresh(local_win, rowpadnav,padref.padxstart ,  padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);

		 break;
		 case KEY_DOWN:
			if(rowpadnav <=rowcount+1){
				rowpadnav++;}
			prefresh(local_win, rowpadnav,padref.padxstart ,  padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);

		 break; 					
		}	
	}
  /*END############### NAVIGATION OF PROGRAM ##############################END */	
}		

void fah_to_celsius(WINDOW *local_win, int ymax, int xmax)
{
  char charlocal_buff[len];
  float fahr;
  int row,col;
  getbegyx(local_win,row, col);
  //mvwprintw(local_win, row,col, "y is %d, x is %d", row,col); //row is 3 col is 27
  row = 2;
  col = 1;
  mvwprintw(local_win, row++,col, "Input fahrenheit to convert to celsus");
  mvwaddstr(local_win,row,col, "Enter Fahrenheit:"); //row is 3
  fahr=InputF(local_win, charlocal_buff, ymax,xmax); //input fahrenheit using function InputF then transfer to Celc
  row+=3;
  mvwprintw(local_win,row,col,"Celsius Equivalent is: %.2f", ComputeFahtoCel(fahr));
  wrefresh(local_win); 
}		

void celc_to_fahrenheit(WINDOW *local_win, int ymax, int xmax)
{
  char charlocal_buff[len];
  float celc;
  int row,col;
  
  getbegyx(local_win,row, col);

  row = 2;
  col = 1;
  mvwprintw(local_win, row++,col, "Input Celsius to convert to Fahrenheit");
  mvwaddstr(local_win,row,col, "Enter Celsius:"); //row is 3
  celc=InputF(local_win, charlocal_buff, ymax,xmax); //input fahrenheit using function InputF then transfer to Celc
  row+=3;
  mvwprintw(local_win,row,col,"Fahrenheit Equivalent is: %.2f", ComputeCeltoFah(celc));
  wrefresh(local_win); 
}	

 /*InputF is not calculator function but use by other calculator functions for getting input as it 
	contains input checker for interestcal, fahtocel, celtofah. */ 
float InputF(WINDOW *win_c, char pchar[], int ymax, int xmax)
{
  int s=0; 
  int row=0,col=0;
  int point = 0; // 0 means not yet encountered; 1 means encountered once.
 
  float conv;
  
  getyx(win_c,row,col); //row 6 col 17

 
  while((pchar[s] = wgetch(win_c)) != '\n' && s < 50)
	{    
		if(isspace(pchar[s])) //skipped spaces
			continue;			
			
	    if(!isdigit(pchar[s]) && pchar[s] != 127 && pchar[s] != 46) //skip non integer and collect a character again	
			{continue;}
		else if(pchar[s] == 46 ) // encountered a point
			{  	if(point == 0)   //true, if encountered a point for the first time
					point = 1;   
				else
					continue;   //ignore a point(period) if already encountered once
			}
				
		if(pchar[s]==127) //127 is backspace or delete
			{   
				if(col<=17)
				{   col=17;
			    	wmove(win_c,row,col); //current cursor in win_c
			    }
				else
				{ 	
					if(s!=0){
						waddstr(win_c,"\b \b"); //col 18 still being deleted
						--col;
					}	
				}
				     
				
				if(s!=0)
					pchar[--s] ='\0';
				else
					pchar[s]='\0';	
			}
		else
			{ waddch(win_c,pchar[s++]);
			  prefresh(win_c, 0,0,	3,(xmax-1)*0.30,	ymax-3,(xmax-1)*0.98);
			 col++;
		    }		
     }
     pchar[s]='\0';
     conv = atof(pchar);
     return conv;
 }   
 
 char *item_desc(ITEM *current_item)
{ 
	PANEL_DATA *desc_ptrs;
	int flag;
	char desc_text[80];
	char* message;
	
	message = desc_text;
	desc_ptrs = (PANEL_DATA *)malloc(sizeof(PANEL_DATA)); 
	desc_ptrs = (PANEL_DATA *)item_userptr(current_item);
	flag = desc_ptrs->desc_flag;
	
	if(flag == 0){
		sprintf(desc_text, ": Convert Fahrenheit to Celsius");						
		return message;
	}
	else if(flag == 1){
		sprintf(desc_text, ": This computes Celsius converts it to Fahrenheit");
		return message;	
	}	
	else if(flag == 2){
		sprintf(desc_text, ": This computes Loan monthly Interest and total payment"); 
		return message;
	}
	else if(flag== 3){
		sprintf(desc_text, ":Display a table of sales and commissions ");
		return message;
	}	
	else{
		sprintf(desc_text, "no flag is recognized");
		return message;
	}	
}	

 //func_exit(my_menu, my_items, n_choices, window_menu, windows_calc, latest_win); 
 int func_exit(MENU *my_menu, ITEM **my_items, int nchoices, WINDOW* window_menu, WINDOW* windows_calc[], WINDOW* latest_win)
 {   
	 int i;
	 unpost_menu(my_menu);
	 for(i=0;i<nchoices;i++)
			free_item(my_items[i]);	
	 free_menu(my_menu);
	 
	 for(i=0;i<nchoices-1;i++)
			delwin(windows_calc[i]);
	 delwin(window_menu);
	// delwin(margin); //subwindow
	 delwin(latest_win);
	 
	 refresh();
	 //getch();
	 endwin();
     exit(0);	 
 }	 

void bomb(char *message)
 {
	addstr(message);
	refresh(); getch();
	endwin();
	exit(1);
 }
 
 
 void deleteChar(WINDOW* win, WINDOW* parentwin, int* row, int* col, int* charcount, int delboundary, int* DecimalFlag, int* dcnt, int* wcnt, int padflag){
		PAD_PRESH padref;
		padref = get_prefresh();
		
		//Check if backspacing is out of bounds to input field	
			if(*col<=delboundary){
				*col=delboundary;
				wmove(win,*row,delboundary);
			}
			else{
				if(*DecimalFlag==1){	
					if(*dcnt>=0){
						wprintw(win,"\b \b");
						--(*dcnt);
						(*col)--;
						--(*charcount);		
					}
					else if(*dcnt<0){			//meaning negative
						wprintw(win,"\b \b"); 	//erase the decimal point	
						*DecimalFlag=0;
						(*col)--;
						--(*charcount);
					}		
				}
				else if(*DecimalFlag==0){
					if(*wcnt>=0){		
						wprintw(win,"\b \b");
						--(*wcnt);
						(*col)--;
						--(*charcount);
					}
					else 						//wcnt is negative
						*wcnt=0;		
				}
				   if(padflag==padon)
					{prefresh(win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);}	
				   else
					{refresh();
					 wrefresh(win);}
		}		

}	

double inputIntegral(WINDOW* win, WINDOW* parentwin, int maxsize, int padflag){
	 
  //int maxrow, maxcol;
  PAD_PRESH padref;							//for the parentwin
 // getmaxyx(win,maxrow,maxcol);	 
  padref = get_prefresh();
	  
  /*START: External variables for InputIntegral Functions and delete Functions :START */
  int DecimalFlag=0; 				 //values:1 or 0: default value is 0 meaning use integerArray

  /*Arrays for WholeNumbers and DecimalNumbers */
  int WholeNArr[maxsize+1]; //0-7;8 if for NULL
  int DecimalArr[maxsize+1];

  /*counters for the arrays*/
  int wcnt = 0;					//for whole numbers array
  int dcnt = -1;					//for decimal array -1 is for the decimal point
  /*END: External variables for InputIntegral Functions and delete Functions :END */	
  
  int ch,charcount=0,delboundary=0;
  int row,col=0,power;
  double FinalValue;
  getyx(win,row,col); //row is 7, col is 25
  delboundary=col; 
  
  keypad(win,TRUE);
  while((ch=wgetch(win))!='\n') 	 //end loop when  press <ENTER>
  //while((ch=127)!='\n') 
  {  
	 //check character input if reach limit
	 if(charcount>maxsize){
		 if(ch == 127 || ch ==KEY_BACKSPACE){
			deleteChar(win, parentwin, &row, &col, &charcount, delboundary, &DecimalFlag, &dcnt, &wcnt, padon);
		 }	
		else	
			continue;
	 }	
	 //skip non-integers except 'backspace' and 'decimal point'
	 if(!isdigit(ch) && isspace(ch) && (ch !=127) && (ch !=46) && (ch !=8) && ch!=KEY_BACKSPACE)	
	 	continue; 					//skip it; go to the next alteration of the loop
	 else if(isdigit(ch)){
		 wprintw(win,"%c",ch);  	// display character on the screen
		 ++charcount;
		 col++;               //for character input limit
		 
		 /*Check decimal flag: if 1, assign value to Decimal Array */
		 DecimalFlag==1?(DecimalArr[++dcnt]=ch):(WholeNArr[wcnt++]=ch);
		 
		 	if(padflag==padon)
				{prefresh(win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);}
			else	
				{wrefresh(win);}	

	 }
	 else if(ch ==46){ 				//46 is a period/point in ascii
		 if(DecimalFlag==1)			//if decimalFlag is 1 meaning it already encountered a pointa and has not been deleted
			continue;
		 else{
			DecimalFlag=1;			//Set DecimalFlag t0 1: entering decimal Phase	
			wprintw(win,"%c",ch);	//print the period
			++charcount;
			col++;
				if(padflag==padon)
					{prefresh(win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);}
				else	
					{wrefresh(win);}	
		}
	}	 	
	else if(ch==KEY_BACKSPACE || ch==127){
			deleteChar(win, parentwin, &row, &col, &charcount, delboundary, &DecimalFlag, &dcnt, &wcnt, padon);
	}
  }	
  
  //Presses Enter
  WholeNArr[wcnt]='\0';
  DecimalArr[++dcnt]='\0';
  int i;
  for(i=0,FinalValue=0.0;isdigit(WholeNArr[i]);i++)
	FinalValue = 10.0 * FinalValue + (WholeNArr[i]-'0');
	
  for(i=0,power=1;isdigit(DecimalArr[i]);i++){
	FinalValue=10.0 * FinalValue + (DecimalArr[i]-'0');
	power*=10;
  }	
	
  //mvwprintw(win,10,1,"FinalValue=%f, power=%d, Fdivp=%.3f",FinalValue,power,FinalValue/power);	

  return FinalValue/power;	
   	
}	
