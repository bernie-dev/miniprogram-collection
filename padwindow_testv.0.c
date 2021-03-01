
#include <ncurses.h>
#include <stdlib.h>


 void bomb(char *message)
 {
	addstr(message);
	refresh(); getch();
	endwin();
	exit(1);
 }

int main(void)
{
	WINDOW* pad;
	WINDOW* pea;
	int maxrow, maxcol; 
	
	initscr();
	start_color();
	init_pair(1,COLOR_RED,COLOR_CYAN);
	init_pair(2,COLOR_WHITE,COLOR_BLUE);
	init_pair(3,COLOR_WHITE,COLOR_BLACK);
	init_pair(4,COLOR_BLACK,COLOR_WHITE);
	cbreak();
	keypad(stdscr, TRUE);
	
	
	getmaxyx(stdscr, maxrow,maxcol);
	maxcol=maxcol-1;
    printw("row =%d, col=%d ", maxrow,maxcol); //row =24,col=80
    bkgd(COLOR_PAIR(1));
    refresh();
    
    pad = newpad(20,54); //20 is the whole square
    pea = newwin(maxrow/2,maxcol*0.24,2,2); //height,width,startx,starty
    
    keypad(pad,TRUE);
    if( pad == NULL )
	bomb("Unable to create new pad\n");
	
    wprintw(pad,"This is a pad row=%d, col=%d",maxrow,maxcol);
    wbkgd(pad,COLOR_PAIR(2));
    wbkgd(pea,COLOR_PAIR(4) | A_BOLD);
    box(pad,0,0);
    box(pea,0,0);
    //wprintw(pad,"\nThis is a pad row=%d, col=%d",maxrow,maxcol);
	//prefresh(pad, 0, 0, 2, 23, 22, 76);
	//prefresh(pea, 0, 0, 2, 23, 22, 76);
	wrefresh(pea);
	
	
	//case 1: winparam->HEIGHT=rowmax-5, winparam->WIDTH=colmax*0.69, winparam->startx=colmax*0.30, winparam->starty=3;
	//case 2: winparam->HEIGHT=rowmax/2, winparam->WIDTH=colmax*0.27, winparam->startx=1, winparam->starty=3;
	prefresh(pad, 0, 0, 2, (maxcol)*0.30, maxrow-3, (maxcol)*0.97);
	prefresh(pea, 0, 0, 2, (maxcol)*0.30, maxrow-5, (maxcol)*0.69);
	//prefresh(p, mypadpos, 0, 3, 3, TALL-1,WIDE);
	
    wgetch(pad);
	clear();
	refresh();
	
	endwin();
	return 0;
}

