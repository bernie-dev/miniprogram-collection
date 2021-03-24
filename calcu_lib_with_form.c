/* Syntax Modification History: 																  */
/* Bernard Saga: Dec 15, 2020: Fixed the exit function to not crushed the terminal screen 		  */
/* Bernard Saga: Dec 27, 2020: Transfer item_desc function to calcu_function source file		  */
/* Bernard Saga: FEb 02, 2021: Added set_locale for wide characters								  */
/* Bernard Saga: Feb.08, 2021: modified salesman_table.c										  */
/* Bernard Saga: Feb 11, 2021: Added error window  and shadow 	     							  */
/* Bernard Saga: Feb 19, 2021: Added calcu_win variable in struct PANEL_DATA. replace panel_window usage */
/* Bernard Saga: March 17, 2021: Added value shadowFlag in datatype WIN for window shadow creation */
/* Bernard Saga: MArch 23, 2021: Line74-75. Note about screen size								   */  
/*################################################################################################*/

#include "main_with_form.h"


int main(void)
{
   //set locale for wide characters
   setlocale(LC_ALL, ""); 
	
   /* variable field */
   PANEL_DATA *item_ptrs; //pointer to array but will turn to pointer to arrays via calloc
   PAD_PRESH padref;											//external for the value of prefresh parent pad
   WIN winb, menub; /* One for windows menu and the rest for program windows; typedef struct data type */
   WIN padb;
   WINDOW *window_menu;
   WINDOW *windows_calc[4];// 0-computeInterest, 1-FahrenToCelc, 2-CelcToFahren, 3-Salesman TAble
   
   
   PANEL *panel_menu;
   PANEL *panels_calc[4]; // my_windows[0](intererst_calc), my_windows[1](Fahren-C).....etc
 
   ITEM **my_items;
   MENU *my_menu;
   int n_choices, i,c;
   int ymax, xmax;
   int row=0, col=1;
   char *choices[] = {
					    "Fahrenheit to Cel",		//0
					    "Celsius to Fahren",    	//1
					    "Compute Interest",   		//2
					    "Salesman Table",		    //3
					    "Exit",	            	    //4; exit doesnt have panel
					 };		
  			 	
   
   ITEM *cur = NULL; // holds current item
   char *cur_highlight;


  //start of ncurses  
  initscr();
  raw();
  noecho();
  start_color();
  init_color(NEW_COLOR,RED,GREEN,BLUE);//pink
  init_pair(1,COLOR_RED,COLOR_CYAN); //used
  init_pair(2,COLOR_GREEN,COLOR_BLACK);
  init_pair(3,COLOR_WHITE,COLOR_BLUE); //used
  init_pair(SHADW_CLOR,COLOR_WHITE,COLOR_BLACK); //used for window shadow
  init_pair(5,COLOR_BLACK,NEW_COLOR);
  init_pair(6,COLOR_WHITE,COLOR_BLUE);
  init_pair(9,COLOR_BLACK,COLOR_WHITE);  //used
  init_pair(10,COLOR_BLACK,COLOR_WHITE); //used
  init_pair(11,COLOR_RED,COLOR_WHITE); //used
  init_pair(12,COLOR_BLACK,COLOR_RED); //used
  init_pair(13,COLOR_YELLOW,COLOR_BLUE); //for the salesman table instructions
  init_pair(14,COLOR_YELLOW,COLOR_RED);
  init_pair(15,COLOR_RED,COLOR_GREEN);
  init_pair(16,COLOR_YELLOW,COLOR_WHITE);
  init_pair(17,COLOR_BLUE,COLOR_WHITE);
  init_pair(18,COLOR_WHITE,COLOR_RED);
 
  //get size dimension of stdscr screen 
  //normal screen: ymax is 24, xmax is 80
  //full screen: ymax is 39, xmas is 150	
  getmaxyx(stdscr, ymax, xmax);   
  
  /*Set background color for standard screen */
  wbkgd(stdscr, A_NORMAL | COLOR_PAIR(1) | ' '); //bg:cyan;font:red
  
  //gives number of items
  n_choices = ARRAY_SIZE(choices); //gives me 5 (5 if exit and salesman table is included)
  
  //allocate storage for PANEL_DATA item_ptrs, pointer arrays
  item_ptrs = (PANEL_DATA *)calloc(n_choices, sizeof(PANEL_DATA)); //calloc initializes values compared to malloc

  //my_items is array of pointers; set aside storage
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM)); 
  
  //part where item choices gets associated with PANEL_DATA type variable item_ptrs
  for(i=0; i<n_choices-1; i++)
	{	my_items[i]=new_item(choices[i],choices[i]); 
        set_item_userptr(my_items[i], &item_ptrs[i]);
    }
    my_items[i]=new_item(choices[i],choices[i]);
    set_item_userptr(my_items[i],func_exit);   
    
  //create menu; associating items to menu
  my_menu = new_menu((ITEM **)my_items);		
  
  //set menu option not to show the description
  menu_opts_off(my_menu, O_SHOWDESC);

  //stdscr 
  attron(COLOR_PAIR(1));  
  mvprintw(row++,col,"This program contains library of calculators VER 1.0 HEIGHT=%d, WIDTH=%d",ymax,xmax); //row becomes 1
  //mvprintw(row,col,"programmer: Bernard Saga... rowsize= %d, columnsize = %d",ymax,xmax);
  attroff(COLOR_PAIR(1));
  
  /*START#############initiation for each window flags for init_wparam function########################START*/
  /*#*/ winb.flag = 1;   	// flag to use windows program dimesions;     								 /*#*/
        winb.shadowFlag = true;
  /*#*/ menub.flag = 2;   	// flag to use if menu window dimensions								     /*#*/
        menub.shadowFlag = true;
  /*#*/ padb.flag = 5;      //flag for pad windows     													 /*#*/
        padb.shadowFlag = true;
  /*#*/ 																								 /*#*/                                     
  /*#*/                                                                                                  /*#*/
  /*#*/	//setup dimensions 	                                                                             /*#*/
  /*#*/	init_wparam(&winb,ymax,xmax);    	          													 /*#*/
  /*#*/	init_wparam(&menub,ymax,xmax);      	 														 /*#*/
  /*#*/ init_wparam(&padb,ymax,xmax);																	 /*#*/
        
        padref = get_prefresh();		//populate padref
  /*#*/																									 /*#*/	
  /*END#############initiation for each window flags for init_wparam function############################END*/ 
   
  /*START################### CREATION OF MENU WINDOW ###################################################START*/ 
  /*#*/                                                                                                   /*#*/ 
  /*#*/   //create window for menu 																		  /*#*/ 
  /*#*/   window_menu = create_wind(&menub,stdscr);														  /*#*/
  /*#*/																									  /*#*/	
  /*#*/  //set menu window  and sub window																  /*#*/
  /*#*/  set_menu_win(my_menu, window_menu);															  /*#*/
  /*#*/  set_menu_sub(my_menu, derwin(window_menu, 8, 20, 2, 1));							              /*#*/
  /*#*/  //set menu mark																				  /*#*/	
  /*#*/  set_menu_mark(my_menu, "*");																	  /*#*/
  /*#*/																									  /*#*/
  /*#*/  //menu window colors																			  /*#*/
  /*#*/  set_menu_fore(my_menu, COLOR_PAIR(6)); //blue,white; hightlight text will be blue and background hightlight is white 
  /*#*/  set_menu_back(my_menu, COLOR_PAIR(11));  //green,black; background of window will be black and text and borders are green
  /*#*/																									  /*#*/			
  /*#*/ //post a menu																					  /*#*/
  /*#*/ post_menu(my_menu);																				  /*#*/
  /*#*/ wrefresh(window_menu);																			  /*#*/
  /*#*/																									  /*#*/	
  /*END##################### CREATION OF MENU WINDOW ################################################### END */
  
  //create windows for the calculators  
  for(i=0;i<2;i++){
	windows_calc[i] = create_wind(&winb,stdscr);
	}
	
  //create pads for big space programs	
  for(i=2;i<n_choices-1;i++){
	windows_calc[i] = create_wind(&padb,stdscr);
	}


  //background and font color of windows_calc[n]	
  wbkgd(windows_calc[0],COLOR_PAIR(3)); //Fahrenheit2cel
  wbkgd(windows_calc[1],COLOR_PAIR(9)); //cel2Fahrenheit
  wbkgd(windows_calc[2],COLOR_PAIR(17)); //interest_loan
  wbkgd(windows_calc[3],COLOR_PAIR(9));  //salesman table

  //set color background foreground for window menu
  wbkgd(window_menu,COLOR_PAIR(9));
 
 //attach panel for window_menu
  panel_menu = new_panel(window_menu); //stdscr -panel_menu(window_menu) - 
 
 //attach a panel to each calculator windows... 
 for(i=0;i<n_choices-1;i++)  //0-interest,1-fahrenheit,2-choice3
	panels_calc[i] = new_panel(windows_calc[i]); //stdscr-0-1-2... 2 is on the top
	
  //Populate PANEL_DATA item_ptrs variable: set item_panel to designated calculator panels
  for(i=0;i<n_choices-1;i++)
	{ 
		if(i==0) 
		 {	item_ptrs[i].item_panel = panels_calc[i];  //panels
			item_ptrs[i].func_ptrs =  &fah_to_celsius;                  	
			item_ptrs[i].desc_flag = i;
			item_ptrs[i].calcu_win = windows_calc[i];
		 }	
		 else if(i == 1)
		 {	item_ptrs[i].item_panel = panels_calc[i];	
			item_ptrs[i].func_ptrs = &celc_to_fahrenheit;				//&fah_to_celsius;
			item_ptrs[i].desc_flag = i; 
			item_ptrs[i].calcu_win = windows_calc[i];
		 }	
		 else if(i== 2)
		 {
			item_ptrs[i].item_panel = panels_calc[i];	
			item_ptrs[i].func_ptrs = &interest_calc_win;					
			item_ptrs[i].desc_flag = i;
			item_ptrs[i].calcu_win = windows_calc[i];
		 }
		 else if(i== 3)
		 {
			item_ptrs[i].item_panel = panels_calc[i];	
			item_ptrs[i].func_ptrs = &salesman_table; 
			item_ptrs[i].desc_flag = i;
			item_ptrs[i].calcu_win = windows_calc[i];
		 }	 	 
	}
	
		
	
 //make the panel_menu is in the top	
 top_panel(panel_menu);
 update_panels();
 prefresh(windows_calc[2],padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);	//two pads window at the top of the stacks
 prefresh(windows_calc[3],padref.padystart,padref.padxstart,	padref.screenystart,padref.screenxstart,	padref.HEIGHT,padref.WIDTH);	//two pads window at the top of the stacks
 update_panels();
 doupdate();
 refresh();
 	

 keypad(window_menu, TRUE);
 while((c = wgetch(window_menu)) !=KEY_F(4))
 //while((c = 10) !=KEY_F(4))
 {
	 cur = current_item(my_menu);
	 cur_highlight = (char *)item_name(cur);
	 move(LINES-1,1);
	 clrtobot();
	 attron(COLOR_PAIR(1));
	 mvprintw(LINES-1,1, "%s %s", cur_highlight,item_desc(cur));
	 attroff(COLOR_PAIR(1));
	 refresh(); 
	 
	 switch(c)
	 {
	    case KEY_DOWN:
					menu_driver(my_menu, REQ_DOWN_ITEM);
					cur = current_item(my_menu);			
					cur_highlight = (char *)item_name(cur);
					move(LINES-1,1);
					clrtobot();
					attron(COLOR_PAIR(1));
					mvprintw(LINES-1,1, "%s %s", cur_highlight,item_desc(cur));
					attroff(COLOR_PAIR(1)); 
					break;
		case KEY_UP:
					menu_driver(my_menu, REQ_UP_ITEM);
					cur = current_item(my_menu);			
					cur_highlight = (char *)item_name(cur);
					move(LINES-1,1);
					clrtobot();
					attron(COLOR_PAIR(1));
					mvprintw(LINES-1,1, "%s %s", cur_highlight, item_desc(cur));
					attroff(COLOR_PAIR(1)); 
					break;	
		case 10: //Enter	
			{							
   	 		    /*START-###############-VARIABLE DECLARATION-########################-START*/ 
			    /*#*/  WINDOW* win_cur;												    /*#*/	
				/*#*/  PANEL *panel_cur;											    /*#*/
				/*#*/  void (*func_p)(WINDOW *, int,int); 									    /*#*/
			    /*#*/  int width, height, length, winrow;								/*#*/
				/*#*/  PANEL_DATA* ptrs;												/*#*/
				/*END-################-VARIABLE DECLARATION-###########################-END*/
				 	
				/*START-################ If exit item has been chosen in the menu ##########################-START*/
			    /*#*/  if(0==strcmp("Exit", cur_highlight))													  /*#*/	
			    /*#*/		func_exit(my_menu, my_items, n_choices, window_menu, windows_calc, win_cur);      /*#*/
				/*END-################## If exit item has been chosen in the menu ###########################-END*/		
				
				ptrs=(PANEL_DATA *)malloc(sizeof(PANEL_DATA *));					/*get memomy from data heap */ 		
				ptrs = (PANEL_DATA *)item_userptr(cur); 							// gives associated &item_ptrs of cur to PANEL_DATA* ptrs
				//get the associated windows of the current highlighted panel				
				
				length = strlen(cur_highlight);
				
				panel_cur = (PANEL *)ptrs->item_panel;								/*get associated panel of current choice in menu*/
				win_cur = ptrs->calcu_win;
				
				//top_panel(ptrs->item_panel);
				top_panel(panel_cur);
				update_panels();
				wclear(win_cur);
				box(win_cur,0,0);
				wrefresh(win_cur);
				refresh();
				
				//for the window label - OK
				getmaxyx(win_cur, height, width);
				winrow=height;
				mvwprintw(win_cur,height-winrow,(width - length)/2, "%s", cur_highlight); 
				prefresh(win_cur,0,0,	3,(xmax-1)*0.30,	ymax-3,(xmax-1)*0.98);			//for pad windows
				wrefresh(win_cur);															//for non pad windows
				doupdate();
				refresh();
						
				//call the calculator functions that display the content in the windows
				func_p=ptrs->func_ptrs;	 								//functions associated with the item choice and panel window
				curs_set(1);											//to set cursor to visible
				func_p(win_cur,height,width);							//row is 1		
				
				wrefresh(win_cur);
				refresh();					
				update_panels();  //having problem with sigsev
				doupdate();
				break;

			}
     }	 
     //top_panel(panel_menu);
	 wrefresh(window_menu);
	 refresh();				
	 update_panels(); //might be one of the problems sigsevs
	 doupdate();
  }	 
 
  unpost_menu(my_menu);
  for(i=0; i<n_choices; i++)
		free_item(my_items[i]);
  free_menu(my_menu);		
 
 
  refresh(); 
  getch(); 
  endwin();
  
  
  return 0;
  
 }
