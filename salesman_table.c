/*Feb. 1, 2021: Salesman TAble function needs a new separate program*/
/*Feb. 08, 2021: modified number collection function (upgraded version)*/
/*March 15, 2021: Modified windows to become pads 						*/
/*March 16, 2021: Modified max character for InputIntegral for salesman_Table */


#include "main_with_form.h"
#define DIGITSIZE 3
#define maxinput 8
 
//void deleteChar(WINDOW* win, int *row, int *col, int* charcnt, int delbound, int* DecimalFlag, int* dcnt, int* wcnt);
//double inputIntegral(WINDOW* win, int limit);
//void errorMessage(WINDOW* win, int row, int col,char message[]);


void salesman_table(WINDOW *local_win, int ymax, int xmax)
{
   //set locale for wide characters
   setlocale(LC_ALL, ""); 
   
	int totalSman, subHeight=6, subWidth=xmax-2;
	int lwinrow,lwincol;
	//char IntBuf[DIGITSIZE];
	
	WINDOW* subpad1;
	PAD_PRESH padref;
	padref = get_prefresh();
	char message[80];
	//int ch;
	
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
    mvwprintw(local_win,lwinrow++,lwincol,"Enter number of salesman:" );
    prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
    totalSman=inputIntegral(local_win,1,padon);
    if(totalSman<=0){
		sprintf(message,"Error: zero value not accepted. Exiting...");
		errorMessage("Input should be greater than zero");
		//win_border(local_win,3);
		curs_set(0);
		wrefresh(local_win);
		return;
	}	
    mvwprintw(local_win,lwinrow++,lwincol,"Number of Salesman: %d", totalSman);
    prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
    refresh();
    
  /*    
	//getyx(margin,row,col);
	mvwprintw(margin,row,col, "\nEnter now salesman surname\n");
	while((ch = wgetch(margin))!=KEY_F(4)){
			waddch(margin,ch);
			wrefresh(margin);
	}*/	

    wrefresh(local_win);
	refresh();

	//delwin(margin);
	//delwin(local_win);
	
	return;
} 

/*void errorMessage(WINDOW* win, int row, int col, char message[]){

    WINDOW* errwin;
	WIN errdimensions;
	int errrow,errcol,errlen;
	errdimensions.flag=3;
	init_wparam(&errdimensions,row,col);
	errwin=create_wind(&errdimensions,stdscr); //also create borders
	
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
    refresh();
    
}	*/

