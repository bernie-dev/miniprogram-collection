/*Feb. 1, 2021: Salesman TAble function needs a new separate program*/
/*Feb. 08, 20201: modified number collection function (upgraded version)*/

#include "main_with_form.h"
#define DIGITSIZE 3
#define maxinput 8
 
//void deleteChar(WINDOW* win, int *row, int *col, int* charcnt, int delbound, int* DecimalFlag, int* dcnt, int* wcnt);
//double inputIntegral(WINDOW* win, int limit);
void errorMessage(WINDOW* win, int row, int col,char message[]);


void salesman_table(WINDOW *local_win, int ymax, int xmax)
{
   //set locale for wide characters
   setlocale(LC_ALL, ""); 
   
	int totalSman;
	//char IntBuf[DIGITSIZE];
	
	WINDOW* margin;
	int row,col,marginrow,margincol,margin_maxrow,margin_maxcol;
	char message[80];
	//int ch;
	
	getmaxyx(local_win,row,col);
	margin = derwin(local_win,row-3,col-4,2,2);							//Create subwindow for margin purposes
	keypad(margin,TRUE);
	wbkgd(margin,COLOR_PAIR(3));
    scrollok(margin,TRUE);   											//make margin subwindow scrollable   
    //wscrl(margin,6);          


/*START:Instruction inside the window; Beginning of the program*/		
	wattron(margin,COLOR_PAIR(13) |A_BOLD);
	mvwprintw(margin, 0, 0, "This is salesman Table: Output will be the final commission received by each salesman\
based on the products they've selled. (1)Input no. of salesman, input names of salesman base on how many. (2) \
Input how many products, input names of each products. (3)Input how many salesman sells for each product. \n");
	wattroff(margin,COLOR_PAIR(13) | A_BOLD);
/*END:Instruction inside the window; Beginning of the program*/		

    getmaxyx(margin,margin_maxrow,margin_maxcol);
    getyx(margin,marginrow,margincol);    								 //row=7,col=0
    
	mvwprintw(margin,marginrow++,margincol, "Enter how many Salesman: ",margin_maxrow,margin_maxcol); //col=25
	wrefresh(margin);
    totalSman=inputIntegral(margin, local_win, maxinput,padon);
    
    if(totalSman<=0){
		sprintf(message,"Error: zero value not accepted. Exiting...");
		errorMessage(margin,margin_maxrow,margin_maxcol,message);
		wgetch(margin);
		wclear(local_win);
		win_border(local_win,3);
		curs_set(0);
		wrefresh(local_win);
		return;
	}	
	 
    mvwprintw(margin,marginrow,margincol, "Number of salesman: %d",totalSman);
    wscrl(margin,8); 
    wrefresh(margin);
    
	//getyx(margin,row,col);
	/*mvwprintw(margin,row,col, "\nEnter now salesman surname\n");
	while((ch = wgetch(margin))!=KEY_F(4)){
			waddch(margin,ch);
			wrefresh(margin);
	}*/	

    wrefresh(local_win);
	wrefresh(margin);
	refresh();
	wclear(margin);
	//delwin(margin);
	//delwin(local_win);
	
	return;
} 

void errorMessage(WINDOW* win, int row, int col, char message[]){

    WINDOW* errwin;
	WIN errdimensions;
	int errrow,errcol,errlen;
	errdimensions.flag=3;
	init_wparam(&errdimensions,row,col);
	errwin=create_wind(&errdimensions,win); //also create borders
	
	wbkgd(errwin,COLOR_PAIR(15)); //background green
	getmaxyx(errwin,errrow,errcol); 
	wclear(errwin);
	
	errlen=strlen(message);
    //wattron(errwin,COLOR_PAIR(13));
    mvwprintw(errwin,(errrow/2)-2,(errcol-errlen)/2,"%s",message);
    mvwprintw(errwin,(errrow/2)-1,(errcol-13)/2,"Press any key");
    //wattroff(errwin,COLOR_PAIR(13));
    win_border(errwin,3);
    
    wrefresh(errwin);
    wrefresh(win);
    refresh();
    
}	

/*
void deleteChar(WINDOW* win,int* row, int* col, int* charcount, int delboundary, int* DecimalFlag, int* dcnt, int* wcnt){
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
			refresh();
			wrefresh(win);
		}		

}*/

/*
double inputIntegral(WINDOW* win, int maxsize)
{
	 
  //START: External variables for InputIntegral Functions and delete Functions :START 
  int DecimalFlag=0; 				 //values:1 or 0: default value is 0 meaning use integerArray

  //Arrays for WholeNumbers and DecimalNumbers 
  int WholeNArr[maxsize+1]; 		//0-7;8 if for NULL
  int DecimalArr[maxsize+1];

  //counters for the arrays
  int wcnt = 0;					//for whole numbers array
  int dcnt = -1;					//for decimal array -1 is for the decimal point
  //END: External variables for InputIntegral Functions and delete Functions :END 	
  
  int ch,charcount=0,delboundary=0;
  int row,col=0,power;
  double FinalValue;
  getyx(win,row,col); //row is 7, col is 25
  delboundary=col; 
  
  while((ch=wgetch(win))!='\n') 	 //end loop when  press <ENTER>
  //while((ch=127)!='\n') 
  {  
	 //check character input if reach limit
	 if(charcount>maxsize){
		 if(ch == 127 || ch ==KEY_BACKSPACE)
			deleteChar(win, &row, &col, &charcount, delboundary, &DecimalFlag, &dcnt, &wcnt);
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
		 //Check decimal flag: if 1 assign value to Decimal Array 
		 DecimalFlag==1?(DecimalArr[++dcnt]=ch):(WholeNArr[wcnt++]=ch);
		 wrefresh(win);
	 }
	 else if(ch ==46){ 				//46 is a period/point in ascii
		 if(DecimalFlag==1)			//if decimalFlag is 1 meaning it already encountered a pointa and has not been deleted
			continue;
		 else{
			DecimalFlag=1;			//Set DecimalFlag t0 1: entering decimal Phase	
			wprintw(win,"%c",ch);	//print the period
			++charcount;
			col++;
		}
	}	 	
	else if(ch==KEY_BACKSPACE || ch==127){
			deleteChar(win, &row, &col, &charcount, delboundary, &DecimalFlag, &dcnt, &wcnt);
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
   	
}*/
