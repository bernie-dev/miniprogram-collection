#_*_MakeFile_*_

PWD = /home/bernie/Desktop/C_Projects/ncurses_programs/programs_bernard_ncurses/list_calculator_program
CC=gcc
LIB0=-lmenuw
LIB1=-lpanelw
LIB2=-lncursesw
#LIB2=-lncurses
LIB3=-lm

CFLAGS = -Wall -c
LIBS = ${LIB0} ${LIB1} ${LIB2} ${LIB3}


calcu_lib_with_form: formula.o window_creation.o salesman_table.o calcu_functions.o main.o
	${CC} formula.o window_creation.o salesman_table.o calcu_functions.o main.o -o calcu_lib_with_form $(LIBS)

formula.o: formula.c 
	${CC} ${CFLAGS} formula.c -o formula.o $(LIBS)
	
window_creation.o: window_creation.c main_with_form.h
	${CC} ${CFLAGS} window_creation.c -o window_creation.o $(LIBS)

salesman_table.o: salesman_table.c main_with_form.h
	${CC} ${CFLAGS} salesman_table.c -o salesman_table.o $(LIBS) 

calcu_functions.o: calcu_functions.c main_with_form.h
	${CC} ${CFLAGS} calcu_functions.c -o calcu_functions.o $(LIBS)

main.o: calcu_lib_with_form.c main_with_form.h
	${CC} ${CFLAGS} calcu_lib_with_form.c -o main.o $(LIBS)
	
clean:
	rm *.o

debug:
	${CC} -g calcu_lib_with_form.c main_with_form.h calcu_functions.c window_creation.c formula.c salesman_table.c $(LIBS)  

