/*Syntax Modification History: 																*/
/*Bernard Saga: Dec 15, 2020: Fixed the exit function to not crushed the terminal screen 	*/
/*Bernard Saga: Dec 15, 2020: Change paraemters for function_exit prototype 				*/
/*Benard Saga: Feb 11, 2021: remove menu variable parameter in win_border 					*/
/*Bernard Saga: Feb 12, 2021: Remove paramter MENU* in create_wind()						*/


#define _XOPEN_SOURCE_EXTENDED	//for ncursesw, for wide characters; must declared before any preprocessor directives
#include <ncursesw/curses.h> //ncursesw is for utf-8
#include <locale.h> //for setlocale
#include <ncursesw/menu.h>
#include <ncursesw/panel.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define NEW_COLOR 7777
#define SHADW_CLOR 4444
#define RED 1000
#define GREEN 750
#define BLUE 750
#define len 50
#define lentest 200
#define size 3
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4
#define padon 1
#define padoff 0

typedef struct _WIN_struct{
    int startx,starty;  
    int HEIGHT, WIDTH;
    int flag;
}WIN;

typedef struct _PANEL_DATA{
  PANEL *item_panel; //panel for pointer to item. ex. is PANEL_DATA item_panel = panels_calc[i]
  void (*func_ptrs)(WINDOW *,int,int); //pointer to function with pointer to WINDOW arguments returning void
  int desc_flag;
  WINDOW* calcu_win;
}PANEL_DATA; // data type to connect with the items

typedef struct _PAD_PRESH{
 int padystart;
 int padxstart;
 int screenystart;
 int screenxstart;
 int HEIGHT,WIDTH; 		
 int flag;	
}PAD_PRESH;


//function prototype -start
void init_wparam(WIN *pwin, int ymax, int xmax); //setup window dimensions
WINDOW *create_wind(WIN *pwin, WINDOW* parentwin);
void win_border(WINDOW *win, int flag);
void interest_calc_win(WINDOW *win_dow, int,int);
void fah_to_celsius(WINDOW *win_dow, int row, int col);
void celc_to_fahrenheit(WINDOW *local_win, int row, int col);
void salesman_table(WINDOW *,int,int);
float compute(float P, float Rpct, int N);
float ComputeFahtoCel(float fahr);
float ComputeCeltoFah(float Celc);
float InputF(WINDOW *, char array[], int row, int col); 
char *item_desc(ITEM *);
int func_exit(MENU *, ITEM **, int choices, WINDOW*, WINDOW* win[], WINDOW*); //func_exit(my_menu, my_items, n_choices, window_menu, windows_calc, latest_win); 
void description_list(WINDOW *, char *);
void bomb(char *);
void padwin_func(WINDOW* padwin);
void deleteChar(WINDOW* win, WINDOW* parentwin, int *row, int *col, int* charcnt, int delbound, int* DecimalFlag, int* dcnt, int* wcnt, int padflag);
double inputIntegral(WINDOW* win, WINDOW* parentwin, int limit, int padflag);
PAD_PRESH get_prefresh(void); //for prefresh for parent;
//function prototype -end



