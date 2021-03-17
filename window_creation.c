/* ########################## REVISION HISTORY: ############################################*/
/* Feb 02,2021: change window borders to accept wide characters(extended ascii/utf-8)		*/
/* FEb 02, 2021: Window borders are now double lines 										*/
/* Feb 11, 2021: Remove menu paramters in win_border function 								*/
/* Feb 12, 2021: Modified init_wparam() replace ternary to switch-case and added flag 3 	*/
/* Feb 12, 2021: Remove paramter MENU* in create_wind()										*/
/* March 17, 2021: Separate window shadow creation from create_wind							*/

#include "main_with_form.h"


void init_wparam(WIN *winparam, int rowmax, int colmax) //if 1 calculator window; if 2 menu window
{	

  colmax =colmax-1;										//minus 1 for margin purposes
  
  switch(winparam->flag){								//1:normal window; 2,menu window; 3:error window; 4: small pad; 5: big pad
		case 1: winparam->HEIGHT=rowmax-5, winparam->WIDTH=colmax*0.69, winparam->startx=colmax*0.30, winparam->starty=3;
				break;
		case 2: winparam->HEIGHT=rowmax/2, winparam->WIDTH=colmax*0.27, winparam->startx=1, winparam->starty=3;
				break;
		case 3: winparam->HEIGHT=(rowmax/2)-2, winparam->WIDTH=colmax-4, winparam->startx=1, winparam->starty=1;
				break;
		case 4: winparam->HEIGHT=rowmax-5, winparam->WIDTH=colmax*0.69, winparam->startx=colmax*0.30, winparam->starty=3;	
				break;
		case 5: winparam->HEIGHT=rowmax-5, winparam->WIDTH=colmax*0.69, winparam->startx=colmax*0.30, winparam->starty=3;
				// prefresh(windows_calc[3],0,0,	3,(xmax-1)*0.30,	ymax-3,(xmax-1)*0.98);
				break;				
  }				
}	

WINDOW *create_wind(WIN *pwin, WINDOW* parentwin)
{
  WINDOW *local_win;
   
  if(pwin->flag==3)
		local_win=subwin(parentwin,pwin->HEIGHT,pwin->WIDTH,pwin->starty,pwin->startx);
  else if(pwin->flag==4)
		local_win=newpad(20,54);
  else if(pwin->flag==5){
		local_win=newpad(200,pwin->WIDTH);
		if(local_win==NULL)
			 mvwprintw(parentwin,0,0,"Error creating pad");
		}	
  else
		{local_win = newwin(pwin->HEIGHT,pwin->WIDTH,pwin->starty,pwin->startx);}
  
  if(pwin->shadowFlag == true)
	window_shadow(pwin, parentwin);
  
  /*START: Calls win_border functions */	
  if(pwin->flag==1)
	win_border(local_win, 1);
  else if(pwin->flag==2)
    win_border(local_win, 2);  
  else if(pwin->flag==5)
	win_border(local_win, 5);
  else
	{box(local_win,0,0);}
  /*END: Calls win_border functions  */	
  

  return local_win;
} 

void win_border(WINDOW *win, int flag)
{    
   int height, width;	
 
   getmaxyx(win, height, width);
	
   //draw a border around the windows -start	
   mvwadd_wch(win,0,0, WACS_D_ULCORNER); // for ncursesw; upper left corner
   mvwhline_set(win, 0,1, WACS_D_HLINE, width-2); //line upper side ;ncursesw
   mvwadd_wch(win, 0,width-1, WACS_D_URCORNER); //upper right side corner
   mvwvline_set(win, 1, 0, WACS_D_VLINE, height-2); //	left vertical line;ncursew
   mvwadd_wch(win, height-1, 0, WACS_D_LLCORNER); //left bottom corner; ncursesw
   mvwhline_set(win, height-1, 1, WACS_D_HLINE, width-2); //bottom side; ncursesw
   mvwvline_set(win, 1,width-1, WACS_D_VLINE, height-2);  //right side corner line: ncursesw
   mvwadd_wch(win, height-1, width-1, WACS_D_LRCORNER); //lower right corner; ncursesw
   //draw a border around the windows -end
  
   if(flag == 2)
	mvwprintw(win,0,(width - 4)/2, "MENU");
   
}

PAD_PRESH get_prefresh(void){
   int maxrow, maxcol;
   getmaxyx(stdscr,	maxrow, maxcol);
   
	PAD_PRESH padref;
	padref.padystart =0;
	padref.padxstart = 0;
	padref.screenystart = 3;
	padref.screenxstart = (maxcol-1)*0.30;
	padref.HEIGHT = maxrow-3;
	padref.WIDTH = (maxcol-1)*0.98;
	//prefresh(windows_calc[3],0,0,	3,(xmax-1)*0.30,	ymax-3,(xmax-1)*0.98);
	return padref;
}

void window_shadow(WIN *attr, WINDOW *parentwin)
{	
  /*START: shadow creation */
  int i;
  wattron(parentwin,COLOR_PAIR(SHADW_CLOR));

  for(i = (attr->startx+2); i < (attr->startx + attr->WIDTH + 1); i++){   //bottom shadow across
        wmove(parentwin,(attr->starty + attr->HEIGHT), i);
        waddch(parentwin,' ');
  }

  for(i = (attr->starty+1); i < (attr->starty + attr->HEIGHT + 1); i++){ //side right shadow downwards
        wmove(parentwin,i,(attr->startx + attr->WIDTH));
        waddch(parentwin,' ');
        wmove(parentwin,i,(attr->startx + attr->WIDTH + 1));
        waddch(parentwin,' ');
  }
  wattroff(parentwin,COLOR_PAIR(SHADW_CLOR));
  /*END: shadow creation */	


}
