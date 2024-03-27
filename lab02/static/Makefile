all:
	gcc -Wall -c CollatzLibrary.c 
	ar rcs libCollatzLibrary.a CollatzLibrary.o 
	gcc -Wall main.c libCollatzLibrary.a -o main_static

clean:	
	rm CollatzLibrary.o libCollatzLibrary.a main_static