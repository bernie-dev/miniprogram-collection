/*Febru.01, 2021: Salesman TAble function needs a new separate program        			*/
/*Febru.08, 2021: modified number collection function (upgraded version)      			*/
/*March 15, 2021: Modified windows to become pads 							  			*/
/*March 16, 2021: Modified max character for InputIntegral for salesman_Table 			*/
/*March 18, 2021: Create Fucnction for getting input for how many the salesman 			*/
/*March 23, 2021: Added function getlistnames(),getname() for name collection   		*/ 
/*March 24, 2021: Added delCh() function												*/		
/*March 25, 2021: getname(), delCh,getlistnames(): added/modified PADREF pointers 		*/	
/*March 26, 2021: Added trigger rows to scroll page nth times when near bottom of pad	*/
/*March 28, 2021: make row variable in getlistnames() a pointer variable			 	*/		
/*MArch 29, 2021: Change getSalesmanQtyF to getQtyF()									*/		
/*MArch 29, 2021: Added *message in getnQtyF()											*/	
/*March 29, 2021: Added scrbot_checker() function, 										*/		
/*March 31, 2021: Change getlistnames parameter to accept character limit				*/
/*April 1, 2021: Initialized array pointers for quantity products for each salesman 	*/
/*April 3, 2021: Added non table soft copy of final output								*/
/*April 8, 2021: Added subpad for the final output table								*/
/*April 8, 2021: Added navigation at the end after the table output						*/
/*April 9, 2021: Added dynamic size computation to tablepad window						*/
/*April 13, 2021: Added Table A function												*/
/*April 14, 2021: Fix column and value issue in Table A									*/


#include "main_with_form.h"
#define DIGITSIZE 3
#define MAXINPUT 8
#define MAXCHARNAME 9
#define PricCommCol 2
#define taab "    "

int getQtyF(WINDOW *localwin, int row, int col, PAD_PRESH *padref, char *message); 
void delCh(WINDOW* local_win, int* delrow, int* delcol, int* charcount, int *strindex, int delboundary, PAD_PRESH *padref);
int getname(WINDOW *local_win, char *strname, int maxchar, int boundary, int *delrow, int *delcol, PAD_PRESH *padref);
char **getlistnames(WINDOW *local_win, int *row, int col, int numofmen, int charlimit, PAD_PRESH *padref);
void SalesmanErrorMessage(WINDOW *local_win, int row, int col, char *message, PAD_PRESH *padref);
void scrbot_checker(int *row, PAD_PRESH *padref);
void createTableA(WINDOW* tablepad,int numproduct, int numsman, char **listnames, char **listprodnames, int **QntyPrdctSold, int prodnamecounter);

//GLOBAL VARIABLES
int triggerline, oneline=1, triggerincrement; //for scrolling the pad whenever near bottom screen


void salesman_table(WINDOW *local_win, int ymax, int xmax)
{
   //set locale for wide characters
   setlocale(LC_ALL, ""); 
   
	int totalSman, totalprodname, subHeight=6, subWidth=xmax-2;
	int lwinrow,lwincol;
	//char IntBuf[DIGITSIZE];
	
	
	WINDOW* subpad1;
	WINDOW* tablepad;
	PAD_PRESH padref; //needed by getlistnames, getname, delCh
	padref = get_prefresh();
	char message[80];
	char **listnames;
	char **listprodnames; 
	int **QntyPrdctSold;
	double **PriceComisArr;
	double **FinalResultArr;
	int charlimname = 9, charlimprod=9;
	int i=0,n=0,j=0,k=0;
	int navch,rowpadnav,scrolluplimit;
	int tableHsize, numtableA;
	
	
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
    mvwprintw(local_win,++lwinrow,lwincol,"Number of Salesman: %d pad width=%d", totalSman,xmax);
    prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
    refresh();
    
    //for the salesmen names
    lwinrow+=2;
    mvwprintw(local_win,lwinrow++,lwincol, "Enter Name ID of Salesman (e.g. bernsagax)");
    prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
    listnames=getlistnames(local_win, &lwinrow, lwincol, totalSman, charlimname, &padref);
    refresh();
    //for product count
    lwinrow++;
    scrbot_checker(&lwinrow, &padref);
    totalprodname=getQtyF(local_win, lwinrow,lwincol, &padref, "Enter number of distinct items to be sold:");
    while(totalprodname<=0){
		sprintf(message,"Error: zero value not accepted. Exiting...");
		errorMessage("Input zero not accepted");
		//curs_set(0);
		touchwin(subpad1);
		wrefresh(local_win);
		totalprodname=getQtyF(local_win,lwinrow,lwincol, &padref,"Enter number of distinct items to be sold:");
	}	
    mvwprintw(local_win,++lwinrow,lwincol,"Total number of items: %d", totalprodname);
    scrbot_checker(&lwinrow, &padref);
    prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
    
    //for the collection products string name
    lwinrow++;
    scrbot_checker(&lwinrow, &padref);
    mvwprintw(local_win, ++lwinrow,lwincol, "Enter product names:");
    scrbot_checker(&lwinrow, &padref);
    prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
    ++lwinrow;
    scrbot_checker(&lwinrow, &padref);
    listprodnames=getlistnames(local_win, &lwinrow, lwincol, totalprodname, charlimprod, &padref);
    
    //How many items sold by each salesman for each products?
    lwinrow++;
    scrbot_checker(&lwinrow, &padref);
    
    //initiation: assign memory to pointer array base on totalprodname and totalSman
    QntyPrdctSold=malloc(totalSman * sizeof(*QntyPrdctSold)); // allocate memory to array, holds quantity of product sold by each salesman
    //assign memory to each pointer members of QntyPrdctSold
    for(i=0;i<totalSman;i++){
	   QntyPrdctSold[i]=malloc(totalprodname * sizeof(*QntyPrdctSold));   	
	}	
    
    mvwprintw(local_win, lwinrow,lwincol,"Input Total sold by salesman for each product");
    prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
    for(n=0;n<totalSman;n++) //row
    { for(j=0;j<totalprodname;j++) //col
		{
		   	mvwprintw(local_win, ++lwinrow, lwincol, "%s's total sold for %s: ", *(listnames+n), *(listprodnames+j));
		   	scrbot_checker(&lwinrow, &padref);
		   	prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
		   	QntyPrdctSold[n][j]=inputIntegral(local_win,1,&padref); //for each salesman there are totalprodname values that represents the products
		   	
		}	
		
	}	
    
    //Enter price for each product
    lwinrow+=2;
    scrbot_checker(&lwinrow, &padref);
    //Initiation and assigning memory for array that holds price and commission values
    PriceComisArr=malloc(totalprodname * sizeof(*PriceComisArr));
    for(i=0;i<totalprodname;i++){
	   PriceComisArr[i]=malloc(PricCommCol * sizeof(*PriceComisArr));   	
	}
	mvwprintw(local_win, lwinrow,lwincol,"Input price for each product");
    prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
	for(i=0;i<totalprodname;i++){
		mvwprintw(local_win, ++lwinrow, lwincol, "Price of %s :", listprodnames[i] );
		scrbot_checker(&lwinrow, &padref);
		prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
		PriceComisArr[i][0] = inputIntegral(local_win,5,&padref);
	}		
    
    //Enter commission you get for each product 
    lwinrow+=2;
    scrbot_checker(&lwinrow, &padref);
    mvwprintw(local_win, lwinrow,lwincol,"Input commission number for each product");
    prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
    for(i=0;i<totalprodname;i++){
		mvwprintw(local_win, ++lwinrow, lwincol, "Salesman commission get for %s :", listprodnames[i] );
		scrbot_checker(&lwinrow, &padref);
		prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
		PriceComisArr[i][1] = inputIntegral(local_win,5,&padref);
	}		 
    
    
    //Allocate memory to double pointer array for the final result
    FinalResultArr=malloc(totalSman * sizeof(*FinalResultArr));
    for(i=0;i<totalSman;i++){
		FinalResultArr[i]=malloc(PricCommCol * sizeof(*FinalResultArr));	
	}	
    
    //compute
    for(i=0;i<PricCommCol;i++) //table is only two columns because of ouput is final sum of products and commision
    {   for(j=0;j<totalSman;j++)
		{   for(k=0;k<totalprodname;k++)
			{
			   FinalResultArr[j][i]+=QntyPrdctSold[j][k] * PriceComisArr[k][i];// total sold by salesman when all their products combined
			   //firstly, multiply the price and each individual product of a salesman then add it to Finalresult first column of every row
			   // every row in FinalResultArr represents the salesman
			   //the first column represents final total sold of each salesman and the second column reps final commission they will get
			   
			}	
			
		}	
		
	}	
    
    //Summary of Input 
    //list of salesman, list of product names, list of total of product solds, list of price, list of commission base on quantity not price
    lwinrow+=2;
    scrbot_checker(&lwinrow, &padref);
    
    scrolluplimit = lwinrow; //start of input summary
    
    //for label of summary need to be in center. ymax, xmax
    mvwprintw(local_win, lwinrow, (xmax-13)/2,"Summary Input");
    mvwprintw(local_win, ++lwinrow,lwincol, "List of Salesman's name");
    scrbot_checker(&lwinrow, &padref);
    prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
    //list all the salesman names
    for(i=0;i<totalSman;i++){
	     mvwprintw(local_win, ++lwinrow,lwincol, "%s ", listnames[i]);
	     scrbot_checker(&lwinrow, &padref);
	     prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
	}
	lwinrow+=2;
	scrbot_checker(&lwinrow, &padref);
	mvwprintw(local_win, lwinrow,lwincol, "List of products name, price and ");
	prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
	//list all the product names
    for(i=0,j=0;i<totalprodname;i++){
	     mvwprintw(local_win, ++lwinrow,lwincol, "name: %s   price: %.2f commission: %.2f", listprodnames[i],PriceComisArr[i][j], PriceComisArr[i][j+1]);
	     scrbot_checker(&lwinrow, &padref);
	     prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
				
	}
		
    
    
    //Display TAble
    
    
    /*lwinrow+=2;
    scrbot_checker(&lwinrow, &padref);
    mvwprintw(local_win, lwinrow,lwincol,"Final output");
    prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
    
    for(i=0;i<totalSman;i++){
		mvwprintw(local_win, ++lwinrow,lwincol,"%.2f \t %.2f", FinalResultArr[i][0], FinalResultArr[i][1]);
		scrbot_checker(&lwinrow, &padref);
		prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
		
	}*/	
	
	//April 7, 2021
  //Creation of table
  //assign the table at the top of the screen
  padref.padystart=++lwinrow;
  //row number of tablespad; 12 is the labels,margins
  tableHsize =12 + totalSman + totalprodname + totalSman;
  
   /*START########## Create a Subpad for table output ######################START*/
    if(xmax > 60){		//width size is fullscreen
		tablepad=subpad(local_win,tableHsize,subWidth,lwinrow+1,1);    							
		if(tablepad==NULL)									
			bomb("Unable to create subpad");	
    }
    else{
		numtableA=totalprodname/4; //how many table can be made out of table A
		tableHsize=((totalSman+4)*numtableA)+totalprodname + totalSman + 12;  //row size of tablepad base on number of totalprodname 
		tablepad=subpad(local_win,tableHsize,subWidth,lwinrow+1,1);    							
    	if(tablepad==NULL)									
			bomb("Unable to create subpad");
    }
		
		
      																	
    keypad(tablepad,TRUE);								
    wbkgd(tablepad,COLOR_PAIR(19)); 
    win_border(tablepad,0);
   /*END########### Create a Subpad for table output #########################END*/
   
   //start-table title plus long underline
   mvwprintw(tablepad, 1, (subWidth-12)/2,"Output Table");
   mvwadd_wch(tablepad,2,0, WACS_T_LTEE);
   mvwhline_set(tablepad, 2, 1, WACS_T_HLINE, subWidth-2); 
   mvwadd_wch(tablepad, 2,subWidth-1, WACS_D_RTEE);
   //end table title plus long underline
   
   mvwprintw(tablepad, 3, 1, "Table A: saleseperson and their product sold");
   
   createTableA(tablepad,totalprodname,totalSman,listnames,listprodnames,QntyPrdctSold,0);
   
   prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
   touchwin(tablepad);
   wrefresh(local_win);
   
   
   //set the output screen at the top 
   padref.padystart=scrolluplimit;
   prefresh(local_win,padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
   
   //navigation mode
   /*START############## NAVIGATION OF PROGRAM ##########################START*/
   
   rowpadnav = padref.padystart;
   while((navch=wgetch(local_win))!='q')
   {
		switch(navch)
		{
		 case KEY_UP:
			if(rowpadnav>scrolluplimit)
			   { rowpadnav--;   }
			else
			   { rowpadnav=scrolluplimit; }	
			prefresh(local_win, rowpadnav,padref.padxstart ,  padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
		 break;
		 case KEY_DOWN:
			rowpadnav++;
			prefresh(local_win, rowpadnav,padref.padxstart ,  padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);
		 break; 					
		}	
	}
  /*END############### NAVIGATION OF PROGRAM ##############################END */
    

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

char **getlistnames(WINDOW *local_win, int *row, int col, int numOfmen, int charlimit, PAD_PRESH *padref) {

//for ordinals
 char ordinals[] = {'s','t','n','d','r','d','t','h'};
 char strname[charlimit];
 int j,x,ordvalue,lenname;
 int delrow, delcol;
 int dellef_bndry;
 

 keypad(local_win, TRUE);
 char **listnames=(char **)malloc(numOfmen * sizeof(char *)); //memory set aside
 
 
 for(j=0,x=1;j<numOfmen;j++,x++)
 {
	 //start-scroll function in pad
	 scrbot_checker(row, padref);
	 //end-scroll function in pad
	 
	 ordvalue=x<4? 2*(x-1):6;
	 mvwprintw(local_win, (*row)++, col, "Enter %d%c%c value: ",x, ordinals[ordvalue],ordinals[ordvalue+1]); //for prompt
	 getyx(local_win, delrow, delcol);
     dellef_bndry=delcol;
	 prefresh(local_win,padref->padystart,padref->padxstart,	padref->screenystart,padref->screenxstart,	padref->HEIGHT,padref->WIDTH);
	 
	 if((lenname=getname(local_win, strname, charlimit, dellef_bndry, &delrow, &delcol, padref))>1) // this means name had atleast one character
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

void createTableA(WINDOW* tablepad,int numproduct, int numsman, char **listnames, char **listprodnames, int **QntyPrdctSold, int prodnamecounter)
{   
	int currow, curcol, tpadrow,tpadcol;
	int lengthstr,rightcollimit,lastproduct=0;
	int fieldremaining,productcharlimit=9;
	int productlimit=numproduct;
	int i=0,j=0,proddigit,intcount=0, totprodval=0;
	
	getmaxyx(tablepad, tpadrow, tpadcol);
	rightcollimit=tpadcol-2; //left margin column
	getyx(tablepad, currow, curcol);
    
    ++currow,curcol=1;
    //first line of table, the header of table A
    mvwprintw(tablepad, currow, curcol, "_________|");
    getyx(tablepad, currow,curcol);
    //print table A column title
    for(i=prodnamecounter;i<productlimit;i++){
		getyx(tablepad, currow,curcol);
        if((curcol+productcharlimit+1)>=rightcollimit){ //this checks if the product name column has reach the border width
             lastproduct=i;     //if it does, record the next product name if any and last column plac
             break;
		}	
	    lengthstr=strlen(listprodnames[i]);
	    mvwprintw(tablepad, currow,curcol,"%s",listprodnames[i]); //print the product name
	    fieldremaining=productcharlimit-lengthstr;  //compute maxchar and string length of product name
	    while(fieldremaining>0){    //fill the remaining char with space every name
			wprintw(tablepad," ");
			fieldremaining--;
	    }		
	    wprintw(tablepad, "|");	
  	
	}	
	
	//this is the part where salesman name and product numbers is in column
	//at the start, set one line line below the column labels
	currow++,curcol=1;
	if(lastproduct)
	   totprodval=lastproduct;
    else
	   totprodval=numproduct;	
		
	//for the rows and column
    for(i=0;i<numsman;i++){
		//for the salesman name for every row
		lengthstr=strlen(listnames[i]);
		mvwprintw(tablepad, currow,curcol,"%s",listnames[i]);
		fieldremaining=productcharlimit-lengthstr;  //compute maxchar and string length of product name
	    while(fieldremaining>0){    //fill the remaining char with space every name
			wprintw(tablepad," ");
			fieldremaining--;
	    }		
	    wprintw(tablepad, "|");
	    //end-for the salesman name for every row
        for(j=prodnamecounter;j<totprodval;j++){   
	         //first product value after the row salesman names: column 0
	         proddigit=QntyPrdctSold[i][j]; //get the value
	         //count the digits for character space
	         if(proddigit==0){
	             intcount=1;}
	         else{    
				while(proddigit){
					proddigit/=10;
					intcount++; // digit count
				}	 
	         }
	         wprintw(tablepad,"%d",QntyPrdctSold[i][j]);
	         fieldremaining=productcharlimit-intcount;
	         while(fieldremaining>0){
				 wprintw(tablepad," ");
				 --fieldremaining; 
			 }
			 wprintw(tablepad, "|");
			 intcount=0;	 		   
			   
        }	      
		 currow++;   
	
    }
	if(lastproduct){
			mvwprintw(tablepad, ++currow, 1, "cont...");
			createTableA(tablepad,numproduct,numsman,listnames,listprodnames,QntyPrdctSold,lastproduct);
	}	
   
         
} 
