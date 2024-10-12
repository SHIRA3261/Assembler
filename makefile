assembler: macro.o main.o global.o first_pass.o first_pass_command.o second_pass.o
	gcc -g -ansi -Wall -pedantic macro.o main.o global.o first_pass.o first_pass_command.o second_pass.o -o assembler

main.o: main.c macro.h global.h first_pass.h first_pass_command.h second_pass.h
	gcc -g -c -ansi -Wall -pedantic main.c 

macro.o: macro.c macro.h global.h
	gcc -g -c -ansi -Wall -pedantic macro.c 

global.o: global.c global.h
	gcc -g -c -ansi -Wall -pedantic global.c 

first_pass.o: first_pass.c first_pass.h first_pass_command.h macro.h global.h
	gcc -g -c -ansi -Wall -pedantic first_pass.c 

first_pass_command.o: first_pass_command.c first_pass_command.h global.h
	gcc -g -c -ansi -Wall -pedantic first_pass_command.c 
	
second_pass.o: second_pass.c second_pass.h first_pass.h	global.h
	gcc -g -c -ansi -Wall -pedantic second_pass.c 








